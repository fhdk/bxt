/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/repositories/UserRepository.h"
#include "coro/sync_wait.hpp"
#include "lmdbxx/lmdb++.h"
#include "utilities/lmdb/Environment.h"

namespace bxt::Persistence {

class UserRepository : public bxt::Core::Domain::UserRepository {
    using User = bxt::Core::Domain::User;

public:
    UserRepository(std::shared_ptr<bxt::Utilities::LMDB::Environment> env)
        : m_environment(env) {
        coro::sync_wait([&env, this]() -> coro::task<void> {
            auto txn = co_await env->begin_rw_txn();

            m_db = lmdb::dbi::open(txn->value, "users", MDB_CREATE);

            txn->value.commit();
        }());
    }

    virtual coro::task<TResult> find_by_id_async(const TId id) override;
    virtual coro::task<TResult>
        find_first_async(std::function<bool(const User &)>) override;

    virtual coro::task<TResults>
        find_async(std::function<bool(const User &)>) override;

    virtual coro::task<TResults> all_async() override;

    virtual coro::task<void> add_async(const User entity) override;

    virtual coro::task<void> remove_async(const TId id) override;

    virtual coro::task<void> update_async(const User entity) override;

private:
    std::shared_ptr<bxt::Utilities::LMDB::Environment> m_environment;
    lmdb::dbi m_db;
};

} // namespace bxt::Persistence
