/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/services/AuthService.di.h"
#include "core/application/services/AuthService.h"

#include <drogon/HttpController.h>

namespace bxt::UI {

class AuthController : public drogon::HttpController<AuthController, false> {
public:
    AuthController(Core::Application::AuthService& service)
        : m_service(service) {}

public:
    METHOD_LIST_BEGIN

    ADD_METHOD_TO(AuthController::auth, "/auth", drogon::Post);
    ADD_METHOD_TO(AuthController::verify, "/verify", drogon::Get);

    METHOD_LIST_END

    drogon::Task<drogon::HttpResponsePtr> auth(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> verify(drogon::HttpRequestPtr req);

private:
    Core::Application::AuthService& m_service;
};

} // namespace bxt::UI

struct AuthControllerService
    : kgr::shared_service<
          bxt::UI::AuthController,
          kgr::dependency<bxt::Core::Application::di::AuthService>> {};
