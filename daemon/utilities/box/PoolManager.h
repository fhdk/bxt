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

#include <filesystem>
#include <mutex>
#include <set>
#include <shared_mutex>
#include <string>
#include <system_error>
#include <vector>
namespace bxt::Box {

class PoolManager {
public:
    struct FsError : public bxt::Error {
        FsError(const std::error_code& ec) { message = ec.message(); }
    };
    BXT_DECLARE_RESULT(FsError);

    PoolManager(const std::filesystem::path& pool_path,
                const std::set<std::string> architectures);
    enum class PoolLocation { Unknown, Sync, Overlay, Automated };

    Result<std::filesystem::path> move_to(const std::filesystem::path& from,
                                          PoolLocation location,
                                          const std::string& arch);

    Result<std::vector<std::filesystem::path>>
        packages(PoolLocation location, const std::string& arch);

private:
    constexpr static frozen::unordered_map<PoolLocation, frozen::string, 3>
        m_location_paths = {{PoolLocation::Sync, "/sync/"},
                            {PoolLocation::Overlay, "/overlay/"},
                            {PoolLocation::Automated, "/automated/"}};

    std::filesystem::path m_pool_path;
    std::set<std::string> m_architectures;
    std::shared_mutex m_pool_mutex;
};

} // namespace bxt::Box