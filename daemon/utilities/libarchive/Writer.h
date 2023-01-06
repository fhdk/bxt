/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "Exception.h"
#include "Header.h"

#include <archive.h>
#include <filesystem>
#include <memory>
#include <ostream>
#include <vector>

struct archive;
struct archive_entry;

namespace Archive {

class Writer {
public:
    class Entry {
        friend class Writer;

    public:
        void write(const std::vector<uint8_t>& data);
        void operator>>(const std::vector<uint8_t>& data);

        void finish() { archive_write_finish_entry(m_writer); }

        const Header& header() { return m_header; }

    private:
        Entry() = default;
        archive* m_writer = nullptr;
        Header m_header;
    };

    Writer() = default;

    void open_filename(const std::filesystem::path& path);
    void open_memory(std::vector<std::byte>& byte_array, size_t& used_size);

    operator archive*() { return m_archive.get(); }

    Entry start_write(Header& header);

private:
    static void deleter(archive* a) {
        int status = archive_write_free(a);
        if (status != ARCHIVE_OK) {
            throw LibException(status, archive_error_string(a));
        }
    }
    std::unique_ptr<archive, decltype(&deleter)> m_archive {archive_write_new(),
                                                            &deleter};
};

} // namespace Archive
