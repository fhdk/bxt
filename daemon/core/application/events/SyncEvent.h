/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: %YEAR% Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/events/IntegrationEventBase.h"

namespace bxt::Core::Application::Events {

struct SyncEvent : public IntegrationEventBase {
    bool started = false;
};

} // namespace bxt::Core::Application::Events