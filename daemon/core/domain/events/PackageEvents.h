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
    PackageAdded(const Package& package): package(package){}
    Package package;
};

struct PackageRemoved : public EventBase {
PackageRemoved(const Package::TId& id): id(id){}

    Package::TId id;
};

struct PackageUpdated : public EventBase {

    PackageUpdated(const Package& new_package, const Package& old_package): new_package(new_package), old_package(old_package){}

    Package new_package;

    Package old_package;
};

} // namespace bxt::Core::Domain::Events
