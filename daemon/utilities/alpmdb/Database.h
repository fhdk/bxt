/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once
#include "utilities/alpmdb/Desc.h"

#include <filesystem>
#include <frozen/set.h>
#include <frozen/string.h>
#include <parallel_hashmap/phmap.h>

namespace bxt::Utilities::AlpmDb {

class Database {
public:
    explicit Database(const std::filesystem::path& path = "./",
                      const std::string& name = "")
        : m_path(std::filesystem::absolute(path)), m_name(name) {
        if (name == "") { m_name = m_path.parent_path().filename(); }
    }

    void add(const std::set<std::string>& files);
    void remove(const std::set<std::string>& packages);
    std::set<std::string> packages() const {
        std::set<std::string> packages;
        std::ranges::transform(m_descriptions,
                               std::inserter(packages, packages.begin()),
                               [](const auto& value) { return value.first; });

        return packages;
    }

    void save() const;
    void load();

private:
    bool package_exists(const std::string& package_name) const;
    void create_symlinks() const;

    constexpr static frozen::set<frozen::string, 3>
        supported_package_extensions = {"pkg.tar.gz", "pkg.tar.xz",
                                        "pkg.tar.zst"};

    std::filesystem::path m_path;
    std::string m_name;
    phmap::node_hash_map<std::string, Desc> m_descriptions;
};
} // namespace bxt::Utilities::AlpmDb
