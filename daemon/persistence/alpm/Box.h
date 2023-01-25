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
    Box(ReadOnlyRepositoryBase<Section, Name> &section_repository)
        : m_section_repository(section_repository) {
        auto sections = coro::sync_wait(m_section_repository.all_async());

        for (const auto &section : sections) {
            auto dto = m_section_dto_mapper.map(section);

            auto path_for_section =
                fmt::format("{}/{}", m_options.location, std::string(dto));

            std::filesystem::create_directories(path_for_section);

            m_map.emplace(dto, Utilities::AlpmDb::Database {path_for_section,
                                                            dto.repository});
        }
    };

    virtual Core::Domain::Package
        find_by_id(const boost::uuids::uuid &id) override;
    virtual Core::Domain::Package
        find_first(std::function<bool(const Core::Domain::Package &)>) override;
    virtual std::vector<Core::Domain::Package> find(
        const std::function<bool(const Core::Domain::Package &)> &) override;
    virtual coro::task<std::vector<Core::Domain::Package>> find_async(
        const std::function<bool(const Core::Domain::Package &)> &) override;
    virtual void add(const Core::Domain::Package &entity) override;
    virtual void remove(const Core::Domain::Package &entity) override;

    virtual coro::task<void>
        add_async(const bxt::Core::Domain::Package &entity) override;
    virtual coro::task<void>
        remove_async(const bxt::Core::Domain::Package &entity) override;

    virtual coro::task<void> add_async(
        const std::span<bxt::Core::Domain::Package> &entities) override;

    virtual coro::task<std::vector<Core::Domain::Package>>
        find_by_section_async(
            const Core::Domain::Section &section) const override;

private:
    BoxOptions m_options;
    ReadOnlyRepositoryBase<Section, Name> &m_section_repository;
    Utilities::Mapper<PackageSectionDTO, Section> m_section_dto_mapper;

    phmap::parallel_flat_hash_map<Core::Application::PackageSectionDTO,
                                  Utilities::AlpmDb::Database>
        m_map;

    std::filesystem::path m_root_path;
};

} // namespace bxt::Persistence
