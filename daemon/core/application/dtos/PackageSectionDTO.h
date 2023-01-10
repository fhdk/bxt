/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/entities/Branch.h"
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
};

inline bool operator==(const bxt::Core::Application::PackageSectionDTO& lh,
                       const bxt::Core::Application::PackageSectionDTO& rh) {
    return lh.branch == rh.branch && lh.repository == rh.repository
           && lh.architecture == rh.architecture;
}

} // namespace bxt::Core::Application

namespace {
using namespace bxt::Core::Domain;
using namespace bxt::Core::Application;
} // namespace

template<> struct bxt::Utilities::Mapper<PackageSectionDTO, Package::Section> {
    PackageSectionDTO map(const Package::Section& from) {
        return Core::Application::PackageSectionDTO {
            .branch = from.m_branch.id(),
            .repository = from.m_repo.id(),
            .architecture = from.m_arch.id()};
    }
};

template<>
struct bxt::Utilities::Mapper<Package::Section, PackageSectionDTO> {
    Mapper(RepositoryBase<Branch, Name>& branch_repository,
           RepositoryBase<Repo, Name>& repo_repository,
           RepositoryBase<Architecture, Name>& architecture_repository)
        : m_branch_repository(branch_repository),
          m_repo_repository(repo_repository),
          m_architecture_repository(architecture_repository) {}

    Package::Section map(const PackageSectionDTO& from) {
        return Core::Domain::Package::Section {
            .m_branch = m_branch_repository.find_by_id(from.branch),
            .m_repo = m_repo_repository.find_by_id(from.repository),
            .m_arch = m_architecture_repository.find_by_id(from.architecture)};
    }

private:
    RepositoryBase<Branch, Name>& m_branch_repository;
    RepositoryBase<Repo, Name>& m_repo_repository;
    RepositoryBase<Architecture, Name>& m_architecture_repository;
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
