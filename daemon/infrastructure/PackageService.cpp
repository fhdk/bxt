/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "PackageService.h"

#include "core/application/dtos/PackageDTO.h"
#include "core/application/errors/CrudError.h"
#include "coro/task.hpp"
#include "coro/when_all.hpp"

#include <vector>

bool move_file(const std::filesystem::path &from,
               const std::filesystem::path &to) {
    std::error_code ec;

    std::filesystem::rename(from, to, ec);

    // try copy + remove original
    if (ec) {
        std::filesystem::copy(
            from, to, std::filesystem::copy_options::overwrite_existing, ec);

        std::filesystem::remove(from);
    }

    return ec.value() != 0;
}

namespace bxt::Infrastructure {

coro::task<PackageService::Result<void>> PackageService::commit_transaction(
    const PackageService::Transaction transaction) {
    std::vector<PackageDTO> packages_to_add;
    std::vector<Package::TId> ids_to_remove;

    packages_to_add.reserve(transaction.to_add.size());
    ids_to_remove.reserve(transaction.to_remove.size());

    std::ranges::transform(
        transaction.to_remove, std::back_inserter(ids_to_remove),
        [](const std::pair<PackageSectionDTO, std::string> &value) {
            return Package::TId {SectionDTOMapper::to_entity(value.first),
                                 Name(value.second)};
        });

    std::vector<coro::task<PackageService::Result<void>>> tasks;
    for (const auto &package : transaction.to_add) {
        tasks.push_back(add_package(package));
    }

    co_await coro::when_all(std::move(tasks));
    co_await m_repository.remove_async(ids_to_remove);

    co_await m_repository.commit_async();

    co_return {};
}

coro::task<PackageService::Result<void>>
    PackageService::add_package(const PackageDTO package) {
    std::filesystem::create_directories(m_options.pool(package.section));

    auto deployed_entity = PackageDTOMapper::to_entity(package);

    if (!deployed_entity.has_value()) {
        co_return bxt::make_error<CrudError>(
            CrudError::ErrorType::InvalidArgument);
    }

    auto current_entitites = m_repository.find_by_section(
        SectionDTOMapper::to_entity(package.section));

    if (!current_entitites.has_value()) {
        co_return bxt::make_error<CrudError>(
            CrudError::ErrorType::InvalidArgument);
    }

    auto current_entity =
        std::ranges::find(*current_entitites, package.name, &Package::name);

    if (current_entity != current_entitites->end()
        && deployed_entity->version() <= current_entity->version()) {
        co_return bxt::make_error<CrudError>(
            CrudError::ErrorType::EntityAlreadyExists);
    }
    auto renamed_package = package;

    move_file(package.filepath,
              m_options.pool(package.section) / package.filepath.filename());

    renamed_package.filepath =
        m_options.pool(package.section) / package.filepath.filename();

    if (package.signature_path) {
        const auto signature_path =
            fmt::format("{}/{}.sig", m_options.pool(package.section).string(),
                        package.filepath.filename().string());

        move_file(*package.signature_path, signature_path);

        renamed_package.signature_path = signature_path;
    }

    co_await m_repository.add_async(
        *PackageDTOMapper::to_entity(renamed_package));

    co_return {};
}

coro::task<PackageService::Result<std::vector<PackageDTO>>>
    PackageService::get_packages(const PackageSectionDTO section_dto) const {
    auto section = SectionDTOMapper::to_entity(section_dto);
    std::vector<PackageDTO> result;

    auto result_entities = co_await m_repository.find_by_section_async(section);

    if (!result_entities.has_value()) { co_return result; }

    result.reserve(result_entities->size());
    std::ranges::transform(*result_entities, std::back_inserter(result),
                           PackageDTOMapper::to_dto);

    co_return result;
}
} // namespace bxt::Infrastructure