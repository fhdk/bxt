/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: %YEAR% Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/events/IntegrationEventBase.h"

#include <fmt/format.h>

namespace bxt::Core::Application::Events {

struct SyncEvent : public IntegrationEventBase {
    SyncEvent() = default;
    SyncEvent(bool started) : started(started) {};
    bool started = false;

    virtual std::string message() const {
        return fmt::format("Sync {}", started ? "started" : "stopped");
    }
};

} // namespace bxt::Core::Application::Events