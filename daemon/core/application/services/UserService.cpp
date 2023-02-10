/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "UserService.h"

coro::task<bool>
    bxt::Core::Application::UserService::add_user(const UserDTO user) {
    try {
        auto user_entity = m_entity_mapper.map(user);

        co_await m_repository.add_async(user_entity);
    } catch (...) { co_return false; }
    co_return true;
}

coro::task<bool>
    bxt::Core::Application::UserService::remove_user(const std::string name) {
    try {
        co_await m_repository.remove_async(std::move(name));
    } catch (...) { co_return false; }

    co_return true;
}

coro::task<std::vector<bxt::Core::Application::UserDTO>>
    bxt::Core::Application::UserService::get_users() const {
    auto values = co_await m_repository.all_async();

    std::vector<bxt::Core::Application::UserDTO> result;
    result.reserve(values.size());

    std::ranges::transform(
        values, std::back_inserter(result),
        [this](const auto &user) { return m_dto_mapper.map(user); });

    co_return result;
}
