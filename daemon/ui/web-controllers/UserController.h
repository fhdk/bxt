/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once
#include "core/application/services/UserService.h"

#include <drogon/HttpController.h>
#include <kangaru/service.hpp>

namespace bxt::UI {

class UserController : public drogon::HttpController<UserController, false> {
public:
    METHOD_LIST_BEGIN

    ADD_METHOD_TO(UserController::add_user, "/users/add", drogon::Post);
    ADD_METHOD_TO(UserController::remove_user, "/users/remove", drogon::Post);
    ADD_METHOD_TO(UserController::get_users, "/users", drogon::Get);

    METHOD_LIST_END

    UserController(Core::Application::UserService& service)
        : m_service(service) {};

    drogon::Task<drogon::HttpResponsePtr> add_user(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr>
        remove_user(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> get_users(drogon::HttpRequestPtr req);

private:
    Core::Application::UserService& m_service;
};

} // namespace bxt::UI
