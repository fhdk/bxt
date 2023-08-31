/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "PackageSectionDTO.h"
#include "core/domain/entities/Package.h"
#include "utilities/StaticDTOMapper.h"

#include <filesystem>
#include <optional>
#include <string>

namespace bxt::Core::Application {
struct PackageDTO {
    PackageSectionDTO section;
    std::string name;
    std::filesystem::path filepath;
    std::optional<std::filesystem::path> signature_path;

    auto operator<=>(const PackageDTO& other) const = default;
};

using PackageDTOMapper =
    bxt::Utilities::StaticDTOMapper<bxt::Core::Domain::Package, PackageDTO>;

} // namespace bxt::Core::Application

namespace {
using namespace bxt::Core::Domain;
using namespace bxt::Core::Application;
} // namespace

template<> struct bxt::Utilities::StaticDTOMapper<Package, PackageDTO> {
    static PackageDTO to_dto(const Package& from) {
        return Core::Application::PackageDTO {
            .section = SectionDTOMapper::to_dto(from.section()),
            .name = from.name(),
            .filepath = from.filepath(),
            .signature_path = from.signature_path()};
    }
    static Package to_entity(const PackageDTO& from) {
        auto section = SectionDTOMapper::to_entity(from.section);
        auto result = Package::from_filepath(section, from.filepath);

        result.set_signature_path(from.signature_path);
        return result;
    }
};
