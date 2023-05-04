/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: %YEAR% Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "EventBase.h"
#include "core/domain/entities/Package.h"

namespace bxt::Core::Domain::Events {

struct PackageAdded : public EventBase {
    Package package;
};

struct PackageRemoved : public EventBase {
    Package::TId id;
};

struct PackageUpdated : public EventBase {
    Package new_package;

    Package old_package;
};

} // namespace bxt::Core::Domain::Events
