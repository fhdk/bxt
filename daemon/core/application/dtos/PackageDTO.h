/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "PackageSectionDTO.h"
#include "core/domain/entities/Package.h"
#include "utilities/Mapper.h"

#include <filesystem>
#include <string>

namespace bxt::Core::Application {
struct PackageDTO {
    PackageSectionDTO section;
    std::string name;
    std::filesystem::path filepath;

    auto operator<=>(const PackageDTO& other) const = default;
};
} // namespace bxt::Core::Application

namespace {
using namespace bxt::Core::Domain;
using namespace bxt::Core::Application;
} // namespace

template<> struct bxt::Utilities::Mapper<PackageDTO, Package> {
    PackageDTO map(const Package& from) const {
        return Core::Application::PackageDTO {
            .section = m_section_mapper.map(from.section()),
            .name = from.name(),
            .filepath = from.filepath()};
    }
    Mapper<PackageSectionDTO, Section> m_section_mapper;
};

template<> struct bxt::Utilities::Mapper<Package, PackageDTO> {
    Package map(const PackageDTO& from) const {
        auto section = m_section_mapper.map(from.section);
        return Package::from_filepath(section, from.filepath);
    }
    Mapper<Section, PackageSectionDTO> m_section_mapper;
};
