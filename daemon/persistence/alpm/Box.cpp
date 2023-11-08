/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "Box.h"

#include "core/application/dtos/PackageDTO.h"
#include "core/application/dtos/PackageSectionDTO.h"
#include "core/domain/entities/Package.h"
#include "core/domain/entities/Section.h"
#include "core/domain/events/PackageEvents.h"
#include "core/domain/repositories/RepositoryBase.h"
#include "coro/sync_wait.hpp"
#include "fmt/core.h"
#include "utilities/Error.h"
#include "utilities/StaticDTOMapper.h"
#include "utilities/alpmdb/Database.h"
#include "utilities/box/Package.h"
#include "utilities/lmdb/Database.h"

#include <fmt/format.h>
#include <functional>
#include <infrastructure/PackageFile.h>
#include <string>
#include <vector>

template<typename T>
inline constexpr std::optional<T>
    boost_opt_to_std(const boost::optional<T> &opt) {
    if (opt.has_value()) {
        return std::make_optional(std::forward<decltype(opt)>(opt).value());
    } else {
        return std::nullopt;
    }
}

template<typename T>
inline constexpr boost::optional<T>
    std_opt_to_boost(const std::optional<T> &opt) {
    if (opt.has_value()) {
        return boost::make_optional(std::forward<decltype(opt)>(opt).value());
    } else {
        return boost::none;
    }
}

template<> struct bxt::Utilities::StaticDTOMapper<Package, bxt::Box::Package> {
    static bxt::Box::Package to_dto(const Package &from) {
        return bxt::Box::Package {.name = from.name(),
                                  .filepath = from.filepath(),
                                  .signature_path =
                                      std_opt_to_boost(from.signature_path()),
                                  .location = from.location()};
    }
    static Package::ParseResult to_entity(const Section &section,
                                          const bxt::Box::Package &from) {
        auto result = Package::from_filepath(section, from.filepath);

        result->set_signature_path(boost_opt_to_std(from.signature_path));
        result->set_pool_location(from.location);
        return result;
    }
};

using BoxPackageMapper =
    bxt::Utilities::StaticDTOMapper<Package, bxt::Box::Package>;

namespace bxt::Persistence {

auto get_sections_from_repository(
    ReadOnlyRepositoryBase<Section> &section_repository) {
    std::vector<PackageSectionDTO> result;
    auto sections = coro::sync_wait(section_repository.all_async());
    if (!sections.has_value()) { return result; }

    result.reserve(sections->size());

    for (const auto &section : *sections) {
        result.emplace_back(SectionDTOMapper::to_dto(section));
    }
    return result;
}

Box::Box(std::shared_ptr<bxt::Utilities::LMDB::Environment> environment,
         std::shared_ptr<coro::io_scheduler> scheduler,
         ReadOnlyRepositoryBase<Section> &section_repository)
    : m_database(environment,
                 scheduler,
                 get_sections_from_repository(section_repository),
                 "bxt::Box") {

    };

coro::task<Box::TResult> Box::find_by_id_async(TId id) {
}

coro::task<Box::TResult>
    Box::find_first_async(std::function<bool(const Package &)>) {
}

coro::task<Box::TResults>
    Box::find_async(std::function<bool(const Package &)> condition) {
}

coro::task<Box::TResults> Box::all_async() {
}

coro::task<Box::WriteResult<void>> Box::add_async(const Package entity) {
    m_to_add.emplace_back(entity);
    co_return {};
}

coro::task<Box::WriteResult<void>> Box::remove_async(const TId entity) {
    m_to_remove.emplace_back(entity);
    co_return {};
}

coro::task<Box::WriteResult<void>>
    Box::add_async(const std::vector<Package> entities) {
    m_to_add.insert(m_to_add.end(), entities.begin(), entities.end());
    co_return {};
}

coro::task<Box::WriteResult<void>> Box::update_async(const Package entity) {
    m_to_update.emplace_back(entity);
    co_return {};
}

coro::task<Box::TResults>
    Box::find_by_section_async(const Core::Domain::Section section) {
    auto packages =
        co_await m_database.find_by_section(SectionDTOMapper::to_dto(section));

    if (!packages.has_value()) {
        co_return bxt::make_error_with_source<ReadError>(
            std::move(packages.error()),
            ReadError::ErrorTypes::InvalidArgument);
    };

    std::vector<Core::Domain::Package> result;
    result.reserve(packages->size());

    for (const auto &package : *packages) {
        auto entity = BoxPackageMapper::to_entity(section, package);

        if (!entity.has_value()) {
            co_return bxt::make_error_with_source<ReadError>(
                std::move(entity.error()),
                ReadError::ErrorTypes::EntityFindError);
        }
        result.emplace_back(*entity);
    }

    co_return result;
}

coro::task<Box::TResults> Box::find_by_section_async(
    const Section section,
    const std::function<bool(const Package &)> predicate) {
    auto packages =
        co_await m_database.find_by_section(SectionDTOMapper::to_dto(section));

    if (!packages.has_value()) {
        co_return bxt::make_error_with_source<ReadError>(
            std::move(packages.error()),
            ReadError::ErrorTypes::InvalidArgument);
    };

    std::vector<Core::Domain::Package> result;
    result.reserve(packages->size());

    for (const auto &package : *packages) {
        auto entity = BoxPackageMapper::to_entity(section, package);
        if (!entity.has_value()) {
            co_return bxt::make_error_with_source<ReadError>(
                std::move(entity.error()),
                ReadError::ErrorTypes::EntityFindError);
        }
        if (predicate(*entity)) { result.emplace_back(*entity); }
    }

    co_return result;
}

coro::task<Box::TResult> Box::find_by_section_async(const Section section,
                                                    const Name name) {
    std::optional<Core::Domain::Package> result;

    co_await m_database.lmdb_handle().accept(
        [&result, &section](std::string_view key, const auto &entity) {
            auto package = BoxPackageMapper::to_entity(section, entity);
            if (!package.has_value()) {
                return Utilities::LMDB::NavigationAction::Next;
            }
            result = *package;

            return Utilities::LMDB::NavigationAction::Stop;
        },
        fmt::format("{}/{}", section.string(), name));

    if (result.has_value()) {
        co_return *result;
    } else {
        co_return bxt::make_error<ReadError>(
            ReadError::ErrorTypes::EntityNotFound);
    }
}

coro::task<UnitOfWorkBase::Result<void>> Box::commit_async() {
    phmap::node_hash_map<PackageSectionDTO, std::set<std::string>> paths_to_add;
    std::vector<coro::task<decltype(m_database)::Result<void>>> tasks;

    tasks.reserve(paths_to_add.size());

    for (const auto &entity : m_to_add) {
        auto dto = BoxPackageMapper::to_dto(entity);
        auto section_dto = SectionDTOMapper::to_dto(entity.section());
        tasks.emplace_back(m_database.add(section_dto, dto));

        auto event = std::make_shared<Events::PackageAdded>(entity);

        m_event_store.emplace_back(event);
    }

    phmap::node_hash_map<PackageSectionDTO, std::set<std::string>>
        names_to_remove;

    /// TODO: Implement update and remove

    co_await coro::when_all(std::move(tasks));

    m_to_add.clear();
    m_to_remove.clear();
    m_to_update.clear();

    co_return {};
}

coro::task<UnitOfWorkBase::Result<void>> Box::rollback_async() {
    m_to_add.clear();
    m_to_remove.clear();
    m_to_update.clear();

    co_return {};
}

} // namespace bxt::Persistence
