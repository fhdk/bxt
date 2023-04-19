/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: %YEAR% Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once
#include "BoostSerializer.h"
#include "Environment.h"

#include <lmdbxx/lmdb++.h>
namespace bxt::Utilities::LMDB {

template<typename T, typename TSerializer = BoostSerializer<T>> class Database {
public:
    Database(std::shared_ptr<Environment> env, std::string_view name = "")
        : m_env(env) {
        auto txn = coro::sync_wait(m_env->begin_rw_txn());

        m_dbi = name.empty() ? lmdb::dbi::open(txn->value)
                             : lmdb::dbi::open(txn->value, name);

        txn->value.commit();
    }

    coro::task<bool> put(std::string_view key, const T& value) {
        auto value_string = TSerializer::serialize(value);

        auto txn = co_await m_env->begin_rw_txn();

        auto result = m_dbi.put(txn->value, key, value_string);

        txn->value.commit();

        co_return result;
    }

    coro::task<bool> del(std::string_view key) {
        auto txn = co_await m_env->begin_rw_txn();

        auto result = m_dbi.del(txn->value, key);

        txn->value.commit();

        co_return result;
    }

    coro::task<std::optional<T>> get(std::string_view key) {
        auto txn = co_await m_env->begin_ro_txn();

        std::string_view value_string;

        if (!m_dbi.get(txn->value, key, value_string)) { co_return {}; }

        co_return TSerializer::deserialize(std::string(value_string));
    }

    lmdb::dbi& dbi() { return m_dbi; }

private:
    std::shared_ptr<Environment> m_env;
    lmdb::dbi m_dbi;
};

} // namespace bxt::Utilities::LMDB
