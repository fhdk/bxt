/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/entities/Package.h"
#include "core/domain/enums/ArchitectureMatch.h"
#include "core/domain/repositories/RepositoryBase.h"

namespace bxt::Core::Domain
{

class PackageService
{
public:
    PackageService();

    static ArchitectureMatch
        match_architectures(const PackageArchitecture& arch,
                            const Name& arch_section);
};

} // namespace bxt::Core::Domain
