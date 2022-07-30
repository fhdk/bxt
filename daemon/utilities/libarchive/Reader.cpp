/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "Reader.h"

#include "Exception.h"

#include <archive.h>

namespace Archive {

void Reader::open_filename(const std::filesystem::path &path) {
    int status =
        archive_read_open_filename(m_archive.get(), path.c_str(), 1024);

    if (status != ARCHIVE_OK) {
        throw LibException(status, archive_error_string(m_archive.get()));
    }
}

void Reader::open_memory(const std::vector<std::byte> &byte_array) {
    int status = archive_read_open_memory(m_archive.get(), byte_array.data(),
                                          byte_array.size());

    if (status != ARCHIVE_OK) {
        throw LibException(status, archive_error_string(m_archive.get()));
    }
}

std::vector<std::byte> Reader::Entry::read(std::size_t amount) {
    if (!m_reader) { throw InvalidEntry(); }

    std::vector<std::byte> result;
    result.resize(amount);

    int size = archive_read_data(m_reader, result.data(), amount);

    if (size < 0) { throw LibException(size, archive_error_string(m_reader)); }

    result.resize(size);

    return result;
}

} // namespace Archive
