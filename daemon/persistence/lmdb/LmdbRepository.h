/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/dtos/UserDTO.h"
#include "core/domain/entities/PackageLogEntry.h"
#include "core/domain/entities/PackageUpdateLogEntry.h"
#include "core/domain/repositories/RepositoryBase.h"
#include "utilities/lmdb/Database.h"
#include "utilities/lmdb/Environment.h"

#include <coro/coro.hpp>

namespace bxt::Persistence {
template<typename TEntity>
class LmdbRepositoryBase : public bxt::Core::Domain::ReadWriteRepositoryBase<TEntity>
{
public:
    LmdbRepositoryBase(
        std::shared_ptr<bxt::Utilities::LMDB::Environment> environment)
        : m_environment(environment), m_db(m_environment) {}

    using TResult =
        typename bxt::Core::Domain::ReadWriteRepositoryBase<TEntity>::TResult;
    using TResults =
        typename bxt::Core::Domain::ReadWriteRepositoryBase<TEntity>::TResults;
    using TId =
        typename bxt::Core::Domain::ReadWriteRepositoryBase<TEntity>::TId;

    virtual coro::task<TResult> find_by_id_async(TId id) override
    {
        //        co_return co_await m_db.get(std::string(id));
    }
    virtual coro::task<TResult>
        find_first_async(std::function<bool(const TEntity &)>) override {}
    virtual coro::task<TResults> find_async(std::function<bool(const TEntity &)> condition) override
    {}
    virtual coro::task<TResults> all_async() override
    {
        TResults results;

        //        auto rotxn = lmdb::txn::begin(m_environment->env(), nullptr, MDB_RDONLY);

        //        {
        //            auto cursor = lmdb::cursor::open(rotxn, m_db.dbi());

        //            std::string_view key, value;
        //            if (cursor.get(key, value, MDB_FIRST)) {
        //                do {
        //                    std::stringstream entity_stream((std::string(value)));
        //                    boost::archive::text_iarchive entity_archive(entity_stream);

        //                    Core::Application::UserDTO usr;
        //                    entity_archive >> usr;

        //                    results.emplace_back(Core::Application::UserDTOMapper::to_entity(usr));
        //                } while (cursor.get(key, value, MDB_NEXT));
        //            }
        //        }

        co_return results;
    }

    virtual coro::task<void> add_async(const TEntity entity) override {
        m_to_add.push_back(entity);
    }
    virtual coro::task<void> update_async(const TEntity entity) override {
        m_to_update.push_back(entity);
    }
    virtual coro::task<void> remove_async(const TId id) override { m_to_remove.push_back(id); }

    virtual coro::task<void> commit_async() override
    {
        for (const auto &el : m_to_add) {
            m_db.put(std::string(el.id()), el);
        }
        for (const auto &el : m_to_remove) {
            m_db.del(el);
        }
    }
    virtual coro::task<void> rollback_async() override
    {
        m_to_add.clear();
        m_to_remove.clear();
        m_to_update.clear();
    }

    virtual std::vector<Core::Domain::Events::EventPtr> event_store() const override
    {
        return m_event_store;
    };

private:
    std::shared_ptr<bxt::Utilities::LMDB::Environment> m_environment;
    Utilities::LMDB::Database<TEntity> m_db;

    std::vector<TEntity> m_to_add;
    std::vector<TId> m_to_remove;
    std::vector<TEntity> m_to_update;

    std::vector<Core::Domain::Events::EventPtr> m_event_store;
};

} // namespace bxt::Persistence
