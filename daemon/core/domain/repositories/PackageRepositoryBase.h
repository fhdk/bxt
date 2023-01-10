/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include <core/domain/entities/Package.h>
#include <core/domain/helpers/RepositoryBase.h>
#include <span>

namespace bxt::Core::Domain {
struct PackageRepositoryBase : public RepositoryBase<Package> {
    virtual coro::task<std::span<Package>>
        find_by_section_async(const Package::Section& section) const = 0;
};
} // namespace bxt::Core::Domain
