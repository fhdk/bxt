/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "coro/when_all.hpp"

#include <boost/uuid/uuid.hpp>
#include <coro/task.hpp>
#include <functional>
#include <span>

namespace bxt::Core::Domain
{

template<typename TEntity, typename TId = boost::uuids::uuid>
struct ReadOnlyRepositoryBase {
    virtual ~ReadOnlyRepositoryBase() = default;

    virtual TEntity find_by_id(const TId& id) = 0;
    virtual TEntity find_first(std::function<bool(const TEntity&)>) = 0;
    virtual std::vector<TEntity> find(std::function<bool(const TEntity&)>) = 0;
    virtual coro::task<std::vector<TEntity>> all_async() = 0;
};

template<typename TEntity, typename TId = boost::uuids::uuid>
struct RepositoryBase {
    virtual ~RepositoryBase() = default;

    virtual TEntity find_by_id(const TId& id) = 0;
    virtual TEntity find_first(std::function<bool(const TEntity&)>) = 0;
    virtual std::vector<TEntity>
        find(const std::function<bool(const TEntity&)>&) = 0;
    virtual coro::task<std::vector<TEntity>>
        find_async(const std::function<bool(const TEntity&)>&) = 0;
    virtual void add(const TEntity& entity) = 0;
    virtual coro::task<void> add_async(const TEntity& entity) = 0;
    virtual coro::task<void> add_async(const std::span<TEntity>& entities) {
        auto make_add_task = [this](TEntity entity) -> coro::task<void> {
            co_return co_await add_async(entity);
        };
        std::vector<coro::task<void>> add_tasks;
        for (const auto& entity : entities) {
            add_tasks.emplace_back(make_add_task(entity));
        }

        co_await coro::when_all(std::move(add_tasks));
        co_return;
    };
    virtual void remove(const TEntity& entity) = 0;
    virtual coro::task<void> remove_async(const TEntity& entity) = 0;
};

} // namespace bxt::Core::Domain
