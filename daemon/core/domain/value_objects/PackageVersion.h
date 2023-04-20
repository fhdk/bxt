/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/value_objects/Name.h"

#include <optional>
#include <string>

#include <fmt/format.h>

namespace bxt::Core::Domain {

struct PackageVersion
{
    static std::strong_ordering compare(const PackageVersion& lh,
                                        const PackageVersion& rh);

    auto operator<=>(const PackageVersion& rh) const {
        return compare(*this, rh);
    };
    std::string string() const;

    std::optional<int> epoch;
    Name version;
    std::string release;
};

} // namespace bxt::Core::Domain
