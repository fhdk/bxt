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
    static int compare(const PackageVersion& lh, const PackageVersion& rh);

    bool operator<(const PackageVersion& other) const
    {
        return compare(*this, other) == -1;
    }

    bool operator>(const PackageVersion& other) const
    {
        return compare(*this, other) == 1;
    }

    bool operator==(const PackageVersion& other) const
    {
        return compare(*this, other) == 0;
    }

    bool operator<=(const PackageVersion& other) const
    {
        auto ret = compare(*this, other);

        return ret == -1 || ret == 0;
    }
    bool operator>=(const PackageVersion& other) const
    {
        auto ret = compare(*this, other);

        return ret == 1 || ret == 0;
    }

    std::string string() const;

    std::optional<int> epoch;
    Name version;
    std::string release;
};

} // namespace bxt::Core::Domain
