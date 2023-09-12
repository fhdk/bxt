/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "Database.h"

#include "nonstd/expected.hpp"
#include "utilities/Error.h"
#include "utilities/libarchive/Error.h"

#include <algorithm>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <coro/latch.hpp>
#include <execution>
#include <fmt/format.h>
#include <iterator>
#include <utilities/libarchive/Reader.h>
#include <utilities/libarchive/Writer.h>

namespace bxt::Utilities::AlpmDb {

coro::task<Database::Result<void>>
    Database::add(const std::set<std::string>& packages) {
    coro::latch latch {static_cast<ptrdiff_t>(packages.size())};

    for (const auto& package : packages) {
        auto description = Desc::parse_package(package);
        if (!description.has_value()) {
            co_return nonstd::unexpected(
                DatabaseError(DatabaseError::ErrorType::InvalidEntityError));
        }
        const auto name = description->get("NAME");

        const auto version = description->get("VERSION");

        if (!name.has_value() || !version.has_value()) { continue; }

        m_descriptions.lazy_emplace_l(
            fmt::format("{}-{}", *name, *version),
            [description, &latch, name](auto& value) {
                value.second = *description;
                latch.count_down();
            },
            [description, &latch, name, version](const auto& ctor) {
                ctor(fmt::format("{}-{}", *name, *version), *description);
                latch.count_down();
            });
    }

    co_await latch;

    co_await save_async();

    co_return {};
}

coro::task<Database::Result<void>>
    Database::remove(const std::set<std::string>& packages) {
    coro::latch latch {static_cast<ptrdiff_t>(packages.size())};

    for (const auto& pkgname : packages) {
        m_descriptions.erase_if(pkgname, [&latch](const auto& value) {
            latch.count_down();
            return true;
        });
    }

    co_await latch;

    co_await save_async();

    co_return {};
}

template<typename TBuffer>
void write_buffer_to_archive(Archive::Writer& writer,
                             const std::string& name,
                             const TBuffer& buffer) {
    auto header = Archive::Header::default_file();

    archive_entry_set_pathname(header, name.c_str());
    archive_entry_set_size(header, buffer.size());

    auto entry = writer.start_write(header);

    if (!entry.has_value()) { return; }

    entry->write({buffer.begin(), buffer.end()});
    entry->finish();
}

coro::task<Database::Result<void>> Database::save_async() {
    auto lock = co_await m_file_lock->lock();

    Archive::Writer db_writer, files_writer;

    std::cout << fmt::format("Adding new entries to {}.db.tar.zst...\n",
                             (m_path / m_name).string());

    archive_write_add_filter_zstd(db_writer);
    archive_write_set_format_pax_restricted(db_writer);

    db_writer.open_filename(m_path / fmt::format("{}.db.tar.zst", m_name));

    archive_write_add_filter_zstd(files_writer);
    archive_write_set_format_pax_restricted(files_writer);
    files_writer.open_filename(m_path
                               / fmt::format("{}.files.tar.zst", m_name));

    for (const auto& [name, description] : m_descriptions) {
        std::cout << fmt::format("Description for {} is being added...\n",
                                 name);

        write_buffer_to_archive(db_writer, fmt::format("{}/desc", name),
                                description.string());

        write_buffer_to_archive(files_writer, fmt::format("{}/files", name),
                                description.files());
    }

    create_symlinks();

    co_return {};
}

void Database::create_symlinks() const {
    std::error_code ec;
    std::filesystem::create_symlink(m_path
                                        / fmt::format("{}.db.tar.zst", m_name),
                                    m_path / fmt::format("{}.db", m_name), ec);

    std::filesystem::create_symlink(
        m_path / fmt::format("{}.files.tar.zst", m_name),
        m_path / fmt::format("{}.files", m_name), ec);
}

coro::task<Database::Result<void>> Database::load() {
    auto lock = co_await m_file_lock->lock();

    Archive::Reader db_reader;

    archive_read_support_format_all(db_reader);
    archive_read_support_filter_all(db_reader);

    const auto opened =
        db_reader.open_filename(m_path / fmt::format("{}.db.tar.zst", m_name));

    if (!opened.has_value()) {
        co_return nonstd::unexpected(DatabaseError {
            DatabaseError::ErrorType::IOError, std::move(opened.error())});
    }

    for (auto& [header, data] : db_reader) {
        std::filesystem::path path = archive_entry_pathname(*header);

        std::vector<std::string> parts;

        for (const auto& part : path) {
            parts.emplace_back(part);
        }

        if (parts.size() != 2) continue;

        if (parts[1] != "desc") continue;

        const auto buffer = data.read_all();
        if (!buffer.has_value()) {
            if (const auto& buffer_err =
                    std::get_if<Archive::InvalidEntryError>(&buffer.error())) {
                co_return nonstd::unexpected(DatabaseError(
                    DatabaseError::ErrorType::DatabaseMalformedError,
                    std::move(*buffer_err)));
            } else {
                co_return nonstd::unexpected(DatabaseError(
                    DatabaseError::ErrorType::DatabaseMalformedError,
                    std::move(*std::get_if<Archive::LibArchiveError>(
                        &buffer.error()))));
            }
        }

        Desc desc({buffer->begin(), buffer->end()});

        m_descriptions[parts[0]] = desc;
    }

    co_return {};
}

} // namespace bxt::Utilities::AlpmDb
