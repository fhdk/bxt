/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "PoolManager.h"

#include "boost/log/trivial.hpp"
#include "fmt/core.h"
#include "nonstd/expected.hpp"
#include "utilities/Error.h"
#include "utilities/box/Package.h"

#include <filesystem>
#include <string>
#include <system_error>
#include <vector>

nonstd::expected<void, std::error_code>
    move_file(const std::filesystem::path& from,
              const std::filesystem::path& to) {
    std::error_code ec;

    std::filesystem::rename(from, to, ec);

    // try copy + remove original
    if (ec) {
        std::filesystem::copy_file(
            from.lexically_normal(), to.lexically_normal(),
            std::filesystem::copy_options::overwrite_existing, ec);
        if (!ec) { std::filesystem::remove(from, ec); }
    }
    if (ec.value() != 0) {
        return nonstd::make_unexpected(ec);
    } else {
        return {};
    }
}

namespace bxt::Box {

std::string PoolManager::format_target_path(
    const std::string& filename,
    bxt::Box::PoolManager::PoolLocation location,
    const std::string& arch) {
    return std::filesystem::absolute(
        fmt::format("{}/{}/{}/{}", m_pool_path.string(),
                    std::string(location_paths.at(location).data(),
                                location_paths.at(location).size()),
                    arch, filename));
}
PoolManager::PoolManager(const std::filesystem::path& pool_path,
                         const std::set<std::string> architectures)
    : m_pool_path(pool_path), m_architectures(architectures) {
    std::error_code ec;
    for (const auto& location : location_paths) {
        for (const auto& architecture : m_architectures) {
            const auto target = std::filesystem::absolute(
                fmt::format("{}/{}/{}", m_pool_path.string(),
                            location.second.data(), architecture));
            std::filesystem::create_directories(target, ec);
        }
    }
}

PoolManager::Result<Package> PoolManager::move_to(const Package& from,
                                                  const std::string& arch) {
    Package result = from;
    for (auto& [location, description] : result.descriptions) {
        std::filesystem::path target = format_target_path(
            description.filepath.filename().string(), location, arch);

        move_file(description.filepath, target);

        description.filepath = target;

        if (!description.signature_path.has_value()) { continue; }

        std::filesystem::path signature_target = format_target_path(
            fmt::format("{}.sig", description.filepath.filename().string()),
            location, arch);

        move_file(*description.signature_path, signature_target);

        description.signature_path = signature_target;
    }
    return result;
}
} // namespace bxt::Box