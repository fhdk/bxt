/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: %YEAR% Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include <chrono>
#include <memory>
namespace bxt::Core::Domain::Events {
struct EventBase {
    std::chrono::time_point<std::chrono::system_clock> when =
        std::chrono::system_clock::now();
};

using EventPtr = std::shared_ptr<EventBase>;

using EmptyEvent = EventBase;

} // namespace bxt::Core::Domain::Events
