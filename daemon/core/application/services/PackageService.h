/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/dtos/PackageSectionDTO.h"
#include "core/domain/repositories/PackageRepositoryBase.h"

#include <coro/task.hpp>

namespace bxt::Core::Application {

class PackageService {
public:
    PackageService(Domain::PackageRepositoryBase& repository)
        : m_repository(repository) {}

    coro::task<std::vector<Domain::Package>>
        get_packages(const PackageSectionDTO& section_dto) const;
    
    

private:
    Domain::PackageRepositoryBase& m_repository;
};

} // namespace bxt::Core::Application
