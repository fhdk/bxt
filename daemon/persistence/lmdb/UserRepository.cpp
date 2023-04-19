/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "UserRepository.h"

#include "core/application/dtos/UserDTO.h"

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
    co_await m_db.put(std::string(entity.id()),
                      Core::Application::UserDTOMapper::to_dto(entity));

    co_return;
}

coro::task<void> UserRepository::remove_async(const TId id) {
    co_await m_db.del(std::string(id));

    co_return;
}

coro::task<void> UserRepository::update_async(const User entity) {
}
} // namespace bxt::Persistence
