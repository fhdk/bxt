/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/entities/User.h"
#include "core/domain/events/EventBase.h"
namespace bxt::Core::Domain::Events {

struct UserAdded : public EventBase {
    User user;
};

struct UserRemoved : public EventBase {
    Name id;
};

struct UserUpdated : public EventBase {
    User new_user;

    User old_user;
};

} // namespace bxt::Core::Domain::Events
