/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: %YEAR% Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/entities/Package.h"
namespace bxt::Core::Domain::Events {

struct PackageAdded {
    Package package;
};

struct PackageRemoved {
    Package package;
};

struct PackageUpdated {
    Package package;

    PackageVersion previous_version;
};

} // namespace bxt::Core::Domain::Events
