/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#pragma once

#include "core/application/dtos/PackageSectionDTO.h"
#include "parallel_hashmap/phmap.h"
#include "utilities/locked.h"

#include <coro/io_scheduler.hpp>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace bxt::Box {
class Database;

class AlpmDbArchiver {
public:
    AlpmDbArchiver(std::shared_ptr<coro::io_scheduler> scheduler,
                   std::vector<Core::Application::PackageSectionDTO> sections,
                   const std::filesystem::path& box_path,
                   Database& db);

    coro::task<void>
        mark_dirty_sections(std::vector<PackageSectionDTO> sections);

private:
    coro::task<void> writeback_to_disk();
    std::filesystem::path m_box_path;

    std::shared_ptr<coro::io_scheduler> m_scheduler;
    std::vector<Core::Application::PackageSectionDTO> m_sections;

    Database& m_db;
    coro::mutex m_mutex;
    phmap::parallel_node_hash_set<PackageSectionDTO> dirty_sections;
    std::atomic<bool> m_scheduled = false;
};

} // namespace bxt::Box