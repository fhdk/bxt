/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: %YEAR% Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "events.h"

#include <coro/task.hpp>
#include <dexode/EventBus.hpp>
#include <memory>
#include <typeindex>

namespace bxt::Utilities {

class EventBusDispatcher {
public:
    EventBusDispatcher(std::shared_ptr<dexode::EventBus> evbus)
        : m_evbus(evbus) {}

    inline void process(bxt::Core::Domain::Events::EventPtr eptr) {
        if (const auto it =
                bxt::events::event_map.find(std::type_index(typeid(*eptr)));
            it != bxt::events::event_map.cend()) {
            auto tindex = std::type_index(typeid(*eptr));
            auto str = tindex.name();
            it->second(eptr, *m_evbus);
        }
    }

    coro::task<void>
        dispatch_async(std::vector<Core::Domain::Events::EventPtr> evlist) {
        if (!m_evbus) co_return;

        for (const auto& event : evlist) {
            process(event);
        }

        m_evbus->process();
    }

private:
    std::shared_ptr<dexode::EventBus> m_evbus;
};

} // namespace bxt::Utilities
