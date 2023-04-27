/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: %YEAR% Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once
#include "EventBusDispatcher.h"
#include "core/domain/repositories/UnitOfWorkBase.h"

namespace bxt::Infrastructure {
template<typename TBase> struct DispatchingUnitOfWork : public TBase {
    virtual coro::task<void> commit_async() {
        TBase::commit_async();

        m_evbus.dispatch_async(TBase::event_store());
    }

protected:
    EventBusDispatcher& m_evbus;
};
} // namespace bxt::Infrastructure
