/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/dtos/PackageSectionDTO.h"
#include "core/domain/repositories/PackageRepositoryBase.h"
#include "persistence/alpm/BoxOptions.h"
#include "utilities/alpmdb/Database.h"

#include <coro/sync_wait.hpp>

namespace bxt::Persistence {

class Box : public Core::Domain::PackageRepositoryBase {
public:
    Box(ReadOnlyRepositoryBase<Section> &section_repository)
        : m_section_repository(section_repository) {
        auto sections = coro::sync_wait(m_section_repository.all_async());

        for (const auto &section : sections) {
            auto dto = SectionDTOMapper::to_dto(section);

            auto path_for_section =
                fmt::format("{}/{}", m_options.location, std::string(dto));

            std::filesystem::create_directories(path_for_section);

            m_map.emplace(dto, Utilities::AlpmDb::Database {path_for_section,
                                                            dto.repository});
        }
    };

    virtual coro::task<TResult> find_by_id_async(TId id) override;
    virtual coro::task<TResult>
        find_first_async(std::function<bool(const Package &)>) override;
    virtual coro::task<TResults>
        find_async(std::function<bool(const Package &)> condition) override;
    virtual coro::task<TResults> all_async() override;

    virtual coro::task<void> add_async(const Package entity) override;
    virtual coro::task<void>
        add_async(const std::vector<Package> entity) override;

    virtual coro::task<void> update_async(const Package entity) override;
    virtual coro::task<void> remove_async(const TId id) override;

    virtual coro::task<std::vector<Package>>
        find_by_section_async(const Section section) const override;

private:
    BoxOptions m_options;
    ReadOnlyRepositoryBase<Section> &m_section_repository;

    phmap::parallel_flat_hash_map<Core::Application::PackageSectionDTO,
                                  Utilities::AlpmDb::Database>
        m_map;

    std::filesystem::path m_root_path;
};

} // namespace bxt::Persistence
