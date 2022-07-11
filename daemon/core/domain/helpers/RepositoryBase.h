/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include <boost/uuid/uuid.hpp>
#include <functional>

namespace bxt::Core::Domain
{

template<typename TEntity, typename TIterable, typename TId = boost::uuids::uuid>
struct ReadOnlyRepositoryBase
{
    virtual ~ReadOnlyRepositoryBase() = default;

    virtual TEntity find_by_id(const TId& id) = 0;
    virtual TEntity find_first(std::function<bool(const TEntity&)>) = 0;
    virtual TIterable find(std::function<bool(const TEntity&)>) = 0;
};

template<typename TEntity, typename TIterable, typename TId = boost::uuids::uuid>
struct RepositoryBase
{
    virtual ~RepositoryBase() = default;

    virtual TEntity find_by_id(const TId& id) = 0;
    virtual TEntity find_first(std::function<bool(const TEntity&)>) = 0;
    virtual TIterable find(std::function<bool(const TEntity&)>) = 0;
    virtual void add(const TEntity& entity) = 0;
    virtual void remove(const TEntity& entity) = 0;
};

} // namespace bxt::Core::Domain
