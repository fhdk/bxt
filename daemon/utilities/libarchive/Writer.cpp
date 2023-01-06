/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "Writer.h"

namespace Archive {

void Writer::open_filename(const std::filesystem::path &path) {
    int status = archive_write_open_filename(m_archive.get(), path.c_str());

    if (status != ARCHIVE_OK) {
        throw LibException(status, archive_error_string(m_archive.get()));
    }
}

void Writer::open_memory(std::vector<std::byte> &byte_array,
                         size_t &used_size) {
    archive_write_open_memory(m_archive.get(), byte_array.data(),
                              byte_array.size(), &used_size);
}

Writer::Entry Writer::start_write(Header &header) {
    archive_write_header(m_archive.get(), header.entry());

    Entry entry;
    entry.m_writer = m_archive.get();

    return entry;
}

void Writer::Entry::write(const std::vector<uint8_t> &data) {
    archive_write_data(m_writer, data.data(), data.size());
}

void Writer::Entry::operator>>(const std::vector<uint8_t> &data) {
    write(data);
}

} // namespace Archive
