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
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

struct archive;
namespace Archive {

class Reader
{
public:
    class Entry {
        friend class Reader;

    public:
        std::vector<std::byte> read(std::size_t amount);
        void skip() { archive_read_data_skip(m_reader); }

    private:
        Entry(archive* a) : m_reader(a) {}
        archive* m_reader = nullptr;
    };

    class Iterator {
        using iterator = Iterator;
        friend class Reader;

    public:
        struct Value {
            std::optional<Header> header;
            Entry entry;
        };
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::size_t;
        using value_type = Value;
        using pointer = value_type*;
        using reference = value_type&;

        Iterator(archive* archive, std::optional<Header> header = std::nullopt)
            : m_value {header, Entry(archive)}, m_archive(archive) {}
        ~Iterator() = default;

        iterator& operator++(int) {
            archive_entry* entry;
            auto status = archive_read_next_header(m_archive, &entry);

            m_value.header = Header(entry);

            if (status != ARCHIVE_OK) { m_value.header = std::nullopt; }
            return *this;
        }

        iterator& operator++() {
            archive_entry* entry;
            auto status = archive_read_next_header(m_archive, &entry);

            m_value.header = Header(entry);

            if (status != ARCHIVE_OK) { m_value.header = std::nullopt; }

            return *this;
        }

        value_type& operator*() { return m_value; }
        value_type& operator->() { return this->operator*(); }

        iterator operator+(std::size_t v) const {
            Header header;
            archive_entry* entry;
            for (std::size_t i = 0; i < v; i++) {
                auto status = archive_read_next_header(m_archive, &entry);

                header = Header(entry);

                if (status != ARCHIVE_OK) {
                    return Iterator {m_archive, std::nullopt};
                }

                archive_read_data_skip(m_archive);
            }

            return {m_archive, header};
        }

        bool operator==(const iterator& rhs) const {
            return this->m_archive == rhs.m_archive
                   && this->m_value.header == rhs.m_value.header;
        }

        bool operator!=(const iterator& rhs) const {
            return !this->operator==(rhs);
        }

    private:
        Value m_value;
        archive* m_archive = nullptr;
    };

    Reader() = default;

    void open_filename(const std::filesystem::path& path);
    void open_memory(const std::vector<std::byte>& byte_array);

    struct archive* archive() {
        return m_archive.get();
    }
    operator struct archive *() { return m_archive.get(); }

    Iterator begin() {
        Iterator it(m_archive.get());
        it.m_archive = m_archive.get();
        it++;

        return it;
    }

    Iterator end() { return Iterator(m_archive.get()); }

private:
    static void deleter(struct archive* a) {
        int status = archive_read_free(a);
        if (status != ARCHIVE_OK) {
            throw LibException(status, archive_error_string(a));
        }
    }
    std::unique_ptr<struct archive, decltype(&deleter)> m_archive {
        archive_read_new(), &deleter};
};

} // namespace Archive

