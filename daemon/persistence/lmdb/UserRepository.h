/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/dtos/UserDTO.h"
#include "core/domain/events/EventBase.h"
#include "core/domain/repositories/RepositoryBase.h"
#include "core/domain/repositories/UnitOfWorkBase.h"
#include "core/domain/repositories/UserRepository.h"
#include "utilities/lmdb/Database.h"
#include "utilities/lmdb/Environment.h"

#include <coro/sync_wait.hpp>
#include <lmdbxx/lmdb++.h>

namespace bxt::Persistence {

class UserRepository : public bxt::Core::Domain::UserRepository {
    using User = bxt::Core::Domain::User;
    template<typename T>
    using WriteResult =
        bxt::Core::Domain::ReadWriteRepositoryBase<User>::template Result<T>;

public:
    UserRepository(std::shared_ptr<bxt::Utilities::LMDB::Environment> env)
        : m_environment(env), m_db(env) {}

    virtual coro::task<TResult> find_by_id_async(const TId id) override;
    virtual coro::task<TResult>
        find_first_async(std::function<bool(const User &)>) override;

    virtual coro::task<TResults>
        find_async(std::function<bool(const User &)>) override;

    virtual coro::task<TResults> all_async() override;

    virtual coro::task<WriteResult<void>> add_async(const User entity) override;

    virtual coro::task<WriteResult<void>> remove_async(const TId id) override;

    virtual coro::task<WriteResult<void>>
        update_async(const User entity) override;

    virtual coro::task<UnitOfWorkBase::Result<void>> commit_async() override;

    virtual coro::task<UnitOfWorkBase::Result<void>> rollback_async() override;

    virtual std::vector<Core::Domain::Events::EventPtr>
        event_store() const override;

private:
    std::shared_ptr<bxt::Utilities::LMDB::Environment> m_environment;
    Utilities::LMDB::Database<Core::Application::UserDTO> m_db;

    std::vector<Core::Domain::User> m_to_add;
    std::vector<TId> m_to_remove;
    std::vector<Core::Domain::User> m_to_update;

    std::vector<Core::Domain::Events::EventPtr> m_event_store;
};

} // namespace bxt::Persistence
