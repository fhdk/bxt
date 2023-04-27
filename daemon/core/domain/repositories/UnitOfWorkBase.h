/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: %YEAR% Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include <coro/sync_wait.hpp>
#include <coro/task.hpp>
#include <functional>

namespace bxt::Core::Domain {
struct UnitOfWorkBase {
    virtual ~UnitOfWorkBase() {}

    virtual coro::task<void> commit_async() = 0;

    virtual coro::task<void> rollback_async() = 0;
};
} // namespace bxt::Core::Domain
