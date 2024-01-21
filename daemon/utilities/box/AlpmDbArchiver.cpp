/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "AlpmDbArchiver.h"

#include "archive.h"
#include "boost/log/trivial.hpp"
#include "coro/task_container.hpp"
#include "utilities/alpmdb/Database.h"
#include "utilities/box/Database.h"
#include "utilities/box/PoolManager.h"
#include "utilities/libarchive/Writer.h"
#include "utilities/lmdb/Database.h"
#include "utilities/log/Logging.h"

#include <coro/sync_wait.hpp>
#include <filesystem>
#include <iterator>
#include <memory>
#include <mutex>
#include <parallel_hashmap/phmap.h>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <utility>

namespace bxt::Box {

AlpmDbArchiver::AlpmDbArchiver(std::shared_ptr<coro::io_scheduler> scheduler,
                               std::vector<PackageSectionDTO> sections,
                               const std::filesystem::path& box_path,
                               Database& db)
    : m_box_path(box_path),
      m_scheduler(scheduler),
      m_sections(sections),
      m_db(db) {
    for (const auto& section : sections) {
        std::filesystem::create_directories(m_box_path / std::string(section));
    }
}

coro::task<void> AlpmDbArchiver::mark_dirty_sections(
    std::vector<PackageSectionDTO> sections) {
    const auto lock = co_await m_mutex.lock();

    dirty_sections.insert(std::make_move_iterator(sections.begin()),
                          std::make_move_iterator(sections.end()));

    if (m_scheduled) { co_return; }

    this->m_scheduled = true;

    m_scheduler->schedule([](auto* self) -> coro::task<void> {
        using namespace std::chrono_literals;

        co_await self->m_scheduler->schedule_after(5s);

        const auto lock = co_await self->m_mutex.lock();

        co_await self->writeback_to_disk();
        self->m_scheduled = false;

        co_return;
    }(this));

    co_return;
}

coro::task<void> AlpmDbArchiver::writeback_to_disk() {
    phmap::parallel_flat_hash_map<PackageSectionDTO, Archive::Writer> writers;

    for (const auto& section : dirty_sections) {
        logd("{}: Writeback started", std::string(section));
        writers.emplace(section, Archive::Writer());

        archive_write_add_filter_zstd(writers.at(section));
        archive_write_set_format_pax_restricted(writers.at(section));

        writers.at(section).open_filename(
            m_box_path / std::string(section)
            / fmt::format("{}.db.tar.zst", section.repository));
    }

    co_await m_db.lmdb_handle().accept(
        [this, &writers](std::string_view key, const Box::Package& package)
            -> Utilities::LMDB::NavigationAction {
            const auto deserialized = Database::deserialize_key(key);

            if (!deserialized.has_value()) {
                return Utilities::LMDB::NavigationAction::Next;
            }

            const auto& [section, name] = *deserialized;

            const auto version = PoolManager::select_preferred_value(package.descriptions)->descfile.get("VERSION");

            if (!version.has_value()) {
                return Utilities::LMDB::NavigationAction::Next;
            }

            if (!writers.contains(section)) {
                return Utilities::LMDB::NavigationAction::Next;
            }

            logd("{}: Description for {}-{} is being added...",
                 std::string(section), name, *version);

            Utilities::AlpmDb::DatabaseUtils::write_buffer_to_archive(
                writers[section], fmt::format("{}-{}/desc", name, *version),
                PoolManager::select_preferred_value(package.descriptions)->descfile.string());

            return Utilities::LMDB::NavigationAction::Next;
        });

    for (const auto& section : dirty_sections) {
        logd("{}: Writeback finished", std::string(section));
    }

    dirty_sections.clear();

    co_return;
}

} // namespace bxt::Box