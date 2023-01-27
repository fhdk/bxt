/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "utilities/alpmdb/PkgInfo.h"

#include <filesystem>
#include <vector>

namespace bxt::Utilities::AlpmDb {

struct DescParseException : public std::runtime_error {
    explicit DescParseException(const std::string& what)
        : std::runtime_error(what) {};
};

class Desc {
public:
    Desc() = default;
    explicit Desc(const std::string& contents, const std::string& files = "");

    static Desc parse_package(const std::filesystem::path& filepath);

    std::string get(const std::string& key) const;

    std::string string() const;
    std::string files() const;

private:
    std::string m_desc;
    std::string m_files;
};
} // namespace bxt::Utilities::AlpmDb
