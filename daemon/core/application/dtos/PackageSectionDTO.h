/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/entities/Package.h"
#include "core/domain/value_objects/Name.h"
#include "utilities/Mapper.h"

#include <boost/functional/hash.hpp>
#include <string>

namespace bxt::Core::Application {
struct PackageSectionDTO {
    std::string branch;
    std::string repository;
    std::string architecture;

    auto operator<=>(const PackageSectionDTO& other) const = default;

    operator std::string() const {
        return fmt::format("{}/{}/{}", branch, repository, architecture);
    }
};

} // namespace bxt::Core::Application

namespace {
using namespace bxt::Core::Domain;
using namespace bxt::Core::Application;
} // namespace

template<> struct bxt::Utilities::Mapper<PackageSectionDTO, Section> {
    PackageSectionDTO map(const Section& from) const {
        return Core::Application::PackageSectionDTO {
            .branch = from.branch(),
            .repository = from.repository(),
            .architecture = from.architecture()};
    }
};

template<> struct bxt::Utilities::Mapper<Section, PackageSectionDTO> {
    Section map(const PackageSectionDTO& from) const {
        return Core::Domain::Section(from.branch, from.repository,
                                     from.architecture);
    }
};

template<> struct std::hash<PackageSectionDTO> {
    std::size_t operator()(PackageSectionDTO const& p) const {
        std::size_t seed = 0;

        boost::hash_combine(seed, p.branch);
        boost::hash_combine(seed, p.repository);
        boost::hash_combine(seed, p.architecture);

        return seed;
    }
};
