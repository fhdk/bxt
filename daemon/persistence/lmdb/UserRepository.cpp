/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "UserRepository.h"

#include "core/application/dtos/UserDTO.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/set.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>

template<class Archive>
void boost::serialization::serialize(Archive &ar,
                                     bxt::Core::Application::UserDTO &user,
                                     const unsigned int version) {
    ar &user.name;
    ar &user.password;
    ar &user.permissions;
}

namespace bxt::Persistence {

coro::task<UserRepository::TResult>
    UserRepository::find_by_id_async(const TId id) {
    auto rotxn = lmdb::txn::begin(m_environment->env(), nullptr, MDB_RDONLY);

    std::string_view entity_string_view;

    auto id_str = std::string(id);

    if (!m_db.get(rotxn, id_str, entity_string_view)) { co_return {}; }

    std::stringstream entity_stream((std::string(entity_string_view)));
    boost::archive::text_iarchive entity_archive(entity_stream);

    Core::Application::UserDTO usr;
    entity_archive >> usr;

    co_return Utilities::Mapper<User, Core::Application::UserDTO>().map(usr);
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
        auto cursor = lmdb::cursor::open(rotxn, m_db);

        std::string_view key, value;
        if (cursor.get(key, value, MDB_FIRST)) {
            do {
                std::stringstream entity_stream((std::string(value)));
                boost::archive::text_iarchive entity_archive(entity_stream);

                Core::Application::UserDTO usr;
                entity_archive >> usr;

                results.emplace_back(
                    Utilities::Mapper<User, Core::Application::UserDTO>().map(
                        usr));
            } while (cursor.get(key, value, MDB_NEXT));
        }
    }

    co_return results;
}

coro::task<void> UserRepository::add_async(const User entity) {
    auto txn = co_await m_environment->begin_rw_txn();

    std::stringstream entity_stream;
    boost::archive::text_oarchive entity_archive(entity_stream);
    entity_archive << Utilities::Mapper<Core::Application::UserDTO, User>().map(
        entity);

    m_db.put(txn->value, std::string(entity.id()), entity_stream.str());
    txn->value.commit();
    co_return;
}

coro::task<void> UserRepository::remove_async(const TId id) {
    auto txn = co_await m_environment->begin_rw_txn();
    m_db.del(txn->value, std::string(id));
    txn->value.commit();
    co_return;
}

coro::task<void> UserRepository::update_async(const User entity) {
}
} // namespace bxt::Persistence
