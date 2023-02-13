/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "utilities/locked.h"

#include <coro/mutex.hpp>
#include <coro/task.hpp>
#include <kangaru/autowire.hpp>
#include <lmdbxx/lmdb++.h>

namespace bxt::Utilities::LMDB {
class Environment {
public:
    Environment() : m_env(lmdb::env::create()) {}

    coro::task<std::unique_ptr<locked<lmdb::txn>>> begin_rw_txn() {
        auto result = std::make_unique<locked<lmdb::txn>>(
            co_await m_mutex.lock(), lmdb::txn::begin(m_env));

        co_return result;
    }

    lmdb::env& env() { return m_env; }

private:
    lmdb::env m_env;
    coro::mutex m_mutex;
};

} // namespace bxt::Utilities::LMDB
