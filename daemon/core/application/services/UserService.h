/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/dtos/UserDTO.h"
#include "core/domain/repositories/UserRepository.h"

namespace bxt::Core::Application {

class UserService
{
public:
    UserService(bxt::Core::Domain::UserRepository& repository)
        : m_repository(repository) {}

    virtual coro::task<bool> add_user(const UserDTO user);
    virtual coro::task<bool> remove_user(const std::string name);
    virtual coro::task<std::vector<UserDTO>> get_users() const;

private:
    bxt::Core::Domain::UserRepository& m_repository;
};

} // namespace bxt::Core::Application
