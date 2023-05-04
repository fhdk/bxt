/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/events/EventBase.h"
#include "core/domain/events/PackageEvents.h"

#include <any>
#include <dexode/EventBus.hpp>
#include <functional>
#include <typeindex>

namespace bxt::events {

using eventbus_visitor =
    std::function<void(bxt::Core::Domain::Events::EventPtr, dexode::EventBus&)>;

template<typename TEvent>
std::pair<std::type_index, eventbus_visitor> to_eventbus_visitor() {
    return {
        std::type_index(typeid(TEvent)),
        [](bxt::Core::Domain::Events::EventPtr event, dexode::EventBus& evbus) {
            evbus.postpone<TEvent>(*(static_cast<TEvent*>(event.get())));
        }};
}

static inline std::unordered_map<std::type_index, eventbus_visitor> event_map {
    to_eventbus_visitor<bxt::Core::Domain::Events::EmptyEvent>(),
    to_eventbus_visitor<bxt::Core::Domain::Events::PackageAdded>(),
    to_eventbus_visitor<bxt::Core::Domain::Events::PackageRemoved>(),
    to_eventbus_visitor<bxt::Core::Domain::Events::PackageUpdated>()};

} // namespace bxt::events
