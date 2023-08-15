/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "UserRepository.h"

#include "core/application/dtos/UserDTO.h"
#include "core/domain/events/UserEvents.h"

#include <boost/uuid/uuid_io.hpp>
#include <iostream>

namespace bxt::Persistence {

coro::task<UserRepository::TResult>
    UserRepository::find_by_id_async(const TId id) {
    auto usr = co_await m_db.get(std::string(id));
    if (!usr) co_return {};

    co_return Core::Application::UserDTOMapper::to_entity(*usr);
}

coro::task<UserRepository::TResult>
    UserRepository::find_first_async(std::function<bool(const User &)>) {
}

coro::task<UserRepository::TResults>
    UserRepository::find_async(std::function<bool(const User &)>) {
}

coro::task<UserRepository::TResults> UserRepository::all_async() {
    UserRepository::TResults results;
    auto rotxn = lmdb::txn::begin(m_environment->env(), nullptr, MDB_RDONLY);

    {
        auto cursor = lmdb::cursor::open(rotxn, m_db.dbi());

        std::string_view key, value;
        if (cursor.get(key, value, MDB_FIRST)) {
            do {
                std::stringstream entity_stream((std::string(value)));
                boost::archive::text_iarchive entity_archive(entity_stream);

                Core::Application::UserDTO usr;
                entity_archive >> usr;

                results.emplace_back(
                    Core::Application::UserDTOMapper::to_entity(usr));
            } while (cursor.get(key, value, MDB_NEXT));
        }
    }

    co_return results;
}

coro::task<void> UserRepository::add_async(const User entity) {
    m_to_add.emplace_back(entity);
    co_return;
}

coro::task<void> UserRepository::remove_async(const TId id) {
    m_to_remove.emplace_back(id);

    co_return;
}

coro::task<void> UserRepository::update_async(const User entity) {
}

coro::task<void> UserRepository::commit_async() {
    std::vector<coro::task<bool>> tasks;

    for (const auto& entity : m_to_add) {
        tasks.emplace_back(
            m_db.put(std::string(entity.id()),
                     Core::Application::UserDTOMapper::to_dto(entity)));

        m_event_store.emplace_back(
            std::make_shared<Core::Domain::Events::UserAdded>(entity));
    }

    for (const auto& entity : m_to_remove) {
        tasks.emplace_back(m_db.del(std::string(entity)));

        m_event_store.emplace_back(
            std::make_shared<Core::Domain::Events::UserRemoved>(entity));
    }

    co_await coro::when_all(std::move(tasks));
}

coro::task<void> UserRepository::rollback_async() {
    m_to_add.clear();
    m_to_remove.clear();
    m_to_update.clear();
}

std::vector<Core::Domain::Events::EventPtr>
    UserRepository::event_store() const {
    return m_event_store;
}

} // namespace bxt::Persistence
