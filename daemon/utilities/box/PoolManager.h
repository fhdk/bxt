/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#pragma once

#include "core/domain/value_objects/PackageArchitecture.h"
#include "frozen/map.h"
#include "frozen/string.h"
#include "frozen/unordered_map.h"
#include "utilities/Error.h"
#include "utilities/errors/Macro.h"

#include <bits/ranges_algo.h>
#include <concepts>
#include <filesystem>
#include <mutex>
#include <optional>
#include <set>
#include <shared_mutex>
#include <string>
#include <system_error>
#include <vector>

namespace bxt::Box {
class Package;

class PoolManager {
public:
    struct FsError : public bxt::Error {
        FsError(const std::error_code& ec) { message = ec.message(); }
    };
    BXT_DECLARE_RESULT(FsError);

    PoolManager(const std::filesystem::path& pool_path,
                const std::set<std::string> architectures);
    enum class PoolLocation { Overlay, Automated, Sync, Unknown };

    Result<Package> move_to(const Package& pkg, const std::string& arch);

    template<typename TMap>
    static std::optional<typename TMap::mapped_type>
        select_preferred_value(const TMap& map) {
        static_assert(std::same_as<typename TMap::key_type, PoolLocation>,
                      "Key should be a PoolLocatrion type");
        if (map.empty()) { return {}; }

        const auto min = std::ranges::min_element(map, {}, [](const auto& el) {
                             return static_cast<int>(el.first);
                         })->first;

        return map.at(min);
    }

    std::string format_target_path(const std::string& filename,
                                   bxt::Box::PoolManager::PoolLocation location,
                                   const std::string& arch);
    constexpr static frozen::unordered_map<PoolLocation, frozen::string, 3>
        location_paths = {{PoolLocation::Sync, "/sync/"},
                          {PoolLocation::Overlay, "/overlay/"},
                          {PoolLocation::Automated, "/automated/"}};

private:
    std::filesystem::path m_pool_path;
    std::set<std::string> m_architectures;
    std::shared_mutex m_pool_mutex;
};

} // namespace bxt::Box