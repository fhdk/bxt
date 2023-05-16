/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: %YEAR% Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/repositories/UnitOfWorkBase.h"
#include "utilities/eventbus/EventBusDispatcher.h"

#include <algorithm>

namespace bxt::Infrastructure {
template<typename TBase> struct DispatchingUnitOfWork : public TBase {
public:
    using TBase::TBase;

    void init_dispatcher(Utilities::EventBusDispatcher& dispatcher) {
        m_dispatcher = &dispatcher;
    }

    virtual coro::task<void> commit_async() {
        co_await TBase::commit_async();

        if (m_dispatcher) {
            co_await m_dispatcher->dispatch_async(TBase::event_store());
        }

        co_return;
    }

private:
    Utilities::EventBusDispatcher* m_dispatcher;
};
} // namespace bxt::Infrastructure