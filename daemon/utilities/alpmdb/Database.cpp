/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "Database.h"

#include <algorithm>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <execution>
#include <fmt/format.h>
#include <iterator>
#include <utilities/libarchive/Reader.h>
#include <utilities/libarchive/Writer.h>

namespace bxt::Utilities::AlpmDb {

void Database::add(const std::set<std::string>& packages) {
    for (const auto& package : packages) {
        auto description = Desc::parse_package(package);
        m_descriptions[description.get("NAME")] = description;
    }
}

void Database::remove(const std::set<std::string>& packages) {
    for (const auto& pkgname : packages) {
        m_descriptions.erase(pkgname);
    }
}

template<typename TBuffer>
void write_buffer_to_archive(Archive::Writer& writer,
                             const std::string& name,
                             const TBuffer& buffer) {
    auto header = Archive::Header::default_file();

    archive_entry_set_pathname(header, name.c_str());
    archive_entry_set_size(header, buffer.size());

    auto entry = writer.start_write(header);
    entry.write({buffer.begin(), buffer.end()});
    entry.finish();
}

void Database::save() const {
    Archive::Writer db_writer, files_writer;

    archive_write_add_filter_zstd(db_writer);
    archive_write_set_format_pax_restricted(db_writer);

    db_writer.open_filename(m_path / fmt::format("{}.db.tar.zst", m_name));

    archive_write_add_filter_zstd(files_writer);
    archive_write_set_format_pax_restricted(files_writer);
    files_writer.open_filename(m_path
                               / fmt::format("{}.files.tar.zst", m_name));

    for (const auto& [name, description] : m_descriptions) {
        write_buffer_to_archive(db_writer, fmt::format("{}/desc", name),
                                description.string());

        write_buffer_to_archive(files_writer, fmt::format("{}/files", name),
                                description.files());
    }
}

void Database::load() {
    Archive::Reader db_reader;

    archive_read_support_format_all(db_reader);
    archive_read_support_filter_all(db_reader);

    db_reader.open_filename(m_path / fmt::format("{}.db.tar.zst", m_name));

    for (auto& [header, data] : db_reader) {
        std::filesystem::path path = archive_entry_pathname(*header);

        std::vector<std::string> parts;

        for (const auto& part : path) {
            parts.emplace_back(part);
        }

        if (parts.size() != 2) continue;

        if (parts[1] != "desc") continue;

        auto buffer = data.read_all();
        Desc desc({buffer.begin(), buffer.end()});

        m_descriptions[parts[0]] = desc;
    }
}

} // namespace bxt::Utilities::AlpmDb
