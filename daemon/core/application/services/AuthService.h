/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once
#include "core/domain/repositories/UserRepository.h"

#include <coro/task.hpp>
#include <jwt-cpp/jwt.h>
#include <kangaru/autowire.hpp>
#include <string>

namespace bxt::Core::Application {

class AuthService {
public:
    AuthService(Domain::UserRepository& user_repository)
        : m_user_repository(user_repository) {}

    coro::task<bool> auth(const std::string& name, const std::string& password);

    friend auto service_map(AuthService const&) -> kgr::autowire_single;

private:
    Domain::UserRepository& m_user_repository;
};

} // namespace bxt::Core::Application
