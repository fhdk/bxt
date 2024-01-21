/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "Section.h"
#include "core/domain/entities/AggregateRoot.h"
#include "core/domain/value_objects/Name.h"
#include "core/domain/value_objects/PackageArchitecture.h"
#include "core/domain/value_objects/PackagePoolEntry.h"
#include "core/domain/value_objects/PackageVersion.h"
#include "parallel_hashmap/phmap.h"
#include "utilities/Error.h"
#include "utilities/box/PoolManager.h"

#include <bits/ranges_algo.h>
#include <filesystem>
#include <nonstd/expected.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>

namespace bxt::Core::Domain {

class Package {
public:
    struct TId {
        Section section;
        Name package_name;
    };

    struct ParseError : public bxt::Error {};
    BXT_DECLARE_RESULT(ParseError)

    const TId id() const { return {m_section, m_name}; }
    const std::string& name() const { return m_name; }
    const PackageVersion version() const {
        return Box::PoolManager::select_preferred_value(m_pool_entries)
            .value()
            .version();
    }
    const std::filesystem::path filepath() const {
        return Box::PoolManager::select_preferred_value(m_pool_entries)
            .value()
            .file_path();
    }
    const Box::PoolManager::PoolLocation location() const {
        return static_cast<Box::PoolManager::PoolLocation>(
            std::ranges::min_element(m_pool_entries, {}, [](const auto& el) {
                return static_cast<int>(el.first);
            })->first);
    }

    Package(Section section, const std::string& name, bool is_any)
        : m_section(std::move(section)), m_name(name), m_is_any_arch(is_any) {}

    static std::optional<std::string>
        parse_file_name(const std::string& filename);

    static Result<Package> from_file_path(
        const Section& section,
        const Box::PoolManager::PoolLocation location,
        const std::filesystem::path& filepath,
        const std::optional<std::filesystem::path>& signature_path = {});

    virtual ~Package() = default;

    std::string string() const { return fmt::format("{}", name()); }

    Section section() const { return m_section; }

    void set_name(const std::string& new_name) { m_name = new_name; }

    void set_section(const Section& new_section) { m_section = new_section; }

    bool is_any_arch() const { return m_is_any_arch; }

    phmap::flat_hash_map<Box::PoolManager::PoolLocation, PackagePoolEntry>&
        pool_entries() {
        return m_pool_entries;
    }

    phmap::flat_hash_map<Box::PoolManager::PoolLocation, PackagePoolEntry>
        pool_entries() const {
        return m_pool_entries;
    }

private:
    Section m_section;

    Name m_name;
    bool m_is_any_arch;
    phmap::flat_hash_map<Box::PoolManager::PoolLocation, PackagePoolEntry>
        m_pool_entries;
};

} // namespace bxt::Core::Domain
