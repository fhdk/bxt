/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: %YEAR% Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include <coro/task.hpp>
#include <dexode/EventBus.hpp>
#include <memory>

namespace bxt::Infrastructure {

class EventBusDispatcher {
public:
    EventBusDispatcher(std::shared_ptr<dexode::EventBus> evbus)
        : m_evbus(evbus) {}

    template<typename EventList>
    coro::task<void> dispatch_async(EventList evlist) {
        if (!m_evbus) co_return;

        for (const auto& event : evlist) {
            m_evbus->postpone(event);
        }

        m_evbus->process();
    }

private:
    std::shared_ptr<dexode::EventBus> m_evbus;
};

} // namespace bxt::Infrastructure
