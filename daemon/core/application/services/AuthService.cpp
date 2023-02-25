/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "AuthService.h"

namespace bxt::Core::Application {

coro::task<bool> AuthService::auth(const std::string &name,
                                   const std::string &password) {
    auto entity = co_await m_user_repository.find_by_id_async(name);

    co_return entity.has_value() && entity->password() == password;
}

coro::task<bool> AuthService::verify(const std::string &token) const {
}

} // namespace bxt::Core::Application
