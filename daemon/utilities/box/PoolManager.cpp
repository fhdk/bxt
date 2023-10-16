/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "PoolManager.h"

#include "boost/log/trivial.hpp"
#include "fmt/core.h"
#include "utilities/Error.h"

#include <filesystem>
#include <system_error>
#include <vector>
namespace bxt::Box {
PoolManager::PoolManager(const std::filesystem::path& pool_path,
                         const std::set<std::string> architectures)
    : m_pool_path(pool_path), m_architectures(architectures) {
    std::error_code ec;
    for (const auto& location : m_location_paths) {
        for (const auto& architecture : m_architectures) {
            const auto target = std::filesystem::absolute(
                fmt::format("{}/{}/{}", m_pool_path.string(),
                            location.second.data(), architecture));
            std::filesystem::create_directories(target, ec);
        }
    }
}

PoolManager::Result<std::filesystem::path>
    PoolManager::move_to(const std::filesystem::path& from,
                         PoolLocation location,
                         const std::string& arch) {
    std::error_code ec;

    std::filesystem::path target = std::filesystem::absolute(
        fmt::format("{}/{}/{}/{}", m_pool_path.string(),
                    std::string(m_location_paths.at(location).data(),
                                m_location_paths.at(location).size()),
                    arch, from.filename().string()));

    std::filesystem::rename(from, target, ec);

    // try copy + remove original
    if (ec) {
        std::filesystem::copy_file(
            from.lexically_normal(), target.lexically_normal(),
            std::filesystem::copy_options::overwrite_existing, ec);
        if (!ec) { std::filesystem::remove(from, ec); }
    }
    if (ec.value() != 0) {
        return bxt::make_error<FsError>(ec);
    } else {
        return target;
    }
}

PoolManager::Result<std::vector<std::filesystem::path>>
    PoolManager::packages(PoolLocation location, const std::string& arch) {
    std::vector<std::filesystem::path> result;

    const auto location_path =
        fmt::format("{}/{}/{}", m_pool_path.string(),
                    m_location_paths.at(location).data(), arch);

    std::error_code ec;
    const auto iterator = std::filesystem::directory_iterator(m_pool_path, ec);
    if (ec.value() != 0) { bxt::make_error<FsError>(ec); }
    for (const auto& entry : iterator) {
        result.emplace_back(entry);
    }

    return result;
}
} // namespace bxt::Box