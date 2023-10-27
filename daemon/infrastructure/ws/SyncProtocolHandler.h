/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#pragma once

#include "core/application/events/SyncEvent.h"
#include "core/application/notifications/NotificationDispatcherBase.h"
#include "dexode/EventBus.hpp"
#include "fmt/chrono.h"
#include "fmt/format.h"
#include "infrastructure/ws/HandlerBase.h"

#include "json/value.h"
#include <memory>
namespace bxt::Infrastructure {
struct SyncHandler : public HandlerBase {
    SyncHandler(std::shared_ptr<dexode::EventBus> evbus)
        : m_listener(dexode::EventBus::Listener::createNotOwning(*evbus)) {
        init();
    }

    virtual Json::Value handle_connection() override {
        Json::Value event_json;

        event_json["type"] = "sync";
        event_json["when"] = fmt::format("{}", m_last_event.when);
        event_json["started"] = m_last_event.started;

        return event_json;
    }

    virtual std::string name() override { return "sync"; }

private:
    void init() {
        m_listener.listen<Core::Application::Events::SyncEvent>(
            [this](const auto& event) {
                m_last_event = event;

                Json::Value event_json;

                event_json["type"] = "sync";
                event_json["when"] = fmt::format("{}", event.when);
                event_json["started"] = event.started;

                m_callback(event_json);
            });
    }
    dexode::EventBus::Listener m_listener;
    Core::Application::Events::SyncEvent m_last_event;
};

} // namespace bxt::Infrastructure