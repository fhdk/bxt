/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "PermissionService.h"

namespace bxt::Core::Application {

coro::task<void> PermissionService::add(const std::string &user_name,
                                        const std::string &permission) {
    auto user = co_await m_repository.find_by_id_async(user_name);

    if (!user) { co_return; }

    auto perms = user->permissions();

    perms.emplace(permission);

    user->set_permissions(perms);

    auto task = m_repository.update_async(std::vector {*user});

    co_await task;

    co_return;
}
coro::task<void> PermissionService::remove(const std::string &user_name,
                                           const std::string &permission) {
    auto user = co_await m_repository.find_by_id_async(user_name);

    if (!user) { co_return; }

    auto perms = user->permissions();
    perms.erase(permission);

    user->set_permissions(perms);

    auto task = m_repository.update_async(std::vector {*user});

    co_await task;

    co_return;
}

coro::task<std::vector<std::string>>
    PermissionService::get(const std::string &user_name) {
    auto user = co_await m_repository.find_by_id_async(user_name);

    if (!user) { co_return {}; }

    std::vector<std::string> result;
    result.reserve(user->permissions().size());

    std::ranges::transform(
        user->permissions(), std::back_inserter(result),
        [](const Domain::Permission &result) { return std::string(result); });

    co_return result;
}

coro::task<bool>
    PermissionService::check(const std::string_view target_permission,
                             const std::string &user_name) {
    auto user = co_await m_repository.find_by_id_async(user_name);

    if (!user) { co_return {}; }

    for (const auto &permission : user->permissions()) {
        m_matcher.match(std::string(target_permission), permission);
    }

    co_return false;
}

} // namespace bxt::Core::Application
