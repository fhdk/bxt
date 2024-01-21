/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "Database.h"

#include "boost/algorithm/string.hpp"
#include "core/application/dtos/PackageDTO.h"
#include "core/application/dtos/PackageSectionDTO.h"
#include "fmt/core.h"
#include "lmdb.h"
#include "lmdbxx/lmdb++.h"
#include "utilities/Error.h"
#include "utilities/alpmdb/Desc.h"
#include "utilities/box/Package.h"
#include "utilities/errors/DatabaseError.h"
#include "utilities/lmdb/Database.h"

#include <optional>
#include <string>
#include <string_view>
namespace bxt::Box {

std::optional<std::pair<PackageSectionDTO, std::string>>
    Database::deserialize_key(const std::string_view key) {
    std::vector<std::string> parts;
    boost::split(parts, key, boost::is_any_of("/"));
    if (parts.size() != 4) { return {}; }

    return std::make_pair(PackageSectionDTO {.branch = parts[0],
                                             .repository = parts[1],
                                             .architecture = parts[2]},
                          parts[3]);
}

std::string Database::serialize_key(PackageSectionDTO section,
                                    std::string name) {
    return fmt::format("{}/{}", std::string(section), name);
}

coro::task<Database::Result<void>>
    Database::add(const PackageSectionDTO section,
                  const Package package_entity) {
    auto moved_package =
        m_manager.move_to(package_entity, section.architecture);

    if (!moved_package.has_value()) {
        co_return bxt::make_error_with_source<DatabaseError>(
            std::move(moved_package.error()),
            DatabaseError::ErrorType::InvalidArgument);
    }

    const auto key = serialize_key(section, moved_package->name);

    auto existing_package = co_await m_db.get(key);
    if (existing_package.has_value()) {
        for (const auto& [location, desc] : moved_package->descriptions) {
            existing_package->descriptions[location] = desc;
        }
        moved_package = *existing_package;
    }

    auto result = co_await m_db.put(key, *moved_package);

    if (!result.has_value()) {
        co_return bxt::make_error_with_source<DatabaseError>(
            std::move(result.error()),
            DatabaseError::ErrorType::InvalidArgument);
    }

    auto mark = m_archiver.mark_dirty_sections({section});
    co_await mark;

    co_return {};
}

coro::task<Database::Result<void>>
    bxt::Box::Database::remove(const PackageSectionDTO section,
                               const std::string name) {
    auto result = co_await m_db.del(serialize_key(section, name));

    if (!result.has_value()) {
        co_return bxt::make_error_with_source<DatabaseError>(
            std::move(result.error()),
            DatabaseError::ErrorType::InvalidArgument);
    }

    auto mark = m_archiver.mark_dirty_sections({section});
    co_await mark;

    co_return {};
}

coro::task<Database::Result<std::vector<Package>>>
    bxt::Box::Database::find_by_section(PackageSectionDTO section) {
    std::vector<Package> result;

    co_await lmdb_handle().accept(
        [this, &result](std::string_view key, const auto& value) {
            result.emplace_back(value);
            return Utilities::LMDB::NavigationAction::Next;
        },
        std::string(section));

    co_return result;
}
} // namespace bxt::Box