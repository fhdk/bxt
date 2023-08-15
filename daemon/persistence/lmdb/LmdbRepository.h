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
#include "coro/task.hpp"
#include "coro/when_all.hpp"
#include "utilities/StaticDTOMapper.h"
#include "utilities/lmdb/Database.h"
#include "utilities/lmdb/Environment.h"

#include <coro/coro.hpp>
#include <vector>

namespace bxt::Persistence {
template<typename TEntity, typename TDTO>
class LmdbRepositoryBase
    : public bxt::Core::Domain::ReadWriteRepositoryBase<TEntity> {
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

    using TMapper = Utilities::StaticDTOMapper<TEntity, TDTO>;

    virtual coro::task<TResult> find_by_id_async(TId id) override {
        //        co_return co_await m_db.get(std::string(id));
    }
    virtual coro::task<TResult>
        find_first_async(std::function<bool(const TEntity &)>) override {}
    virtual coro::task<TResults>
        find_async(std::function<bool(const TEntity &)> condition) override {}
    virtual coro::task<TResults> all_async() override {
        TResults results;

        auto rotxn =
            lmdb::txn::begin(m_environment->env(), nullptr, MDB_RDONLY);

        {
            auto cursor = lmdb::cursor::open(rotxn, m_db.dbi());

            std::string_view key, value;
            if (cursor.get(key, value, MDB_FIRST)) {
                do {
                    TDTO res =
                        Utilities::LMDB::BoostSerializer<TDTO>::deserialize(
                            value);

                    results.emplace_back(TMapper::to_entity(res));
                } while (cursor.get(key, value, MDB_NEXT));
            }
        }

        co_return results;
    }

    virtual coro::task<void> add_async(const TEntity entity) override {
        m_to_add.push_back(entity);
        co_return;
    }
    virtual coro::task<void> update_async(const TEntity entity) override {
        m_to_update.push_back(entity);
        co_return;
    }
    virtual coro::task<void> remove_async(const TId id) override {
        m_to_remove.push_back(id);
    }

    virtual coro::task<void> commit_async() override {
        std::vector<coro::task<bool>> tasks;

        for (const auto &el : m_to_add) {
            tasks.push_back(
                m_db.put(std::string(el.id()), TMapper::to_dto(el)));
        }
        for (const auto &el : m_to_remove) {
            tasks.push_back(m_db.del(el));
        }

        co_await coro::when_all(std::move(tasks));

        co_return;
    }
    virtual coro::task<void> rollback_async() override {
        m_to_add.clear();
        m_to_remove.clear();
        m_to_update.clear();

        co_return;
    }

    virtual std::vector<Core::Domain::Events::EventPtr>
        event_store() const override {
        return m_event_store;
    };

private:
    std::shared_ptr<bxt::Utilities::LMDB::Environment> m_environment;
    Utilities::LMDB::Database<TDTO> m_db;

    std::vector<TEntity> m_to_add;
    std::vector<TId> m_to_remove;
    std::vector<TEntity> m_to_update;

    std::vector<Core::Domain::Events::EventPtr> m_event_store;
};

} // namespace bxt::Persistence
