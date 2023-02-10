/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "UserController.h"

#include "core/application/dtos/UserDTO.h"

namespace bxt::UI {

drogon::Task<drogon::HttpResponsePtr>
    UserController::add_user(drogon::HttpRequestPtr req) {
    auto json = *req->getJsonObject();

    const auto name = json["name"].asString();
    const auto password = json["password"].asString();

    bxt::Core::Application::UserDTO dto {name, password, {}};
    if (!co_await m_service.add_user(dto)) {
        auto response = drogon::HttpResponse::newHttpResponse();
        response->setStatusCode(drogon::k400BadRequest);
        co_return response;
    }

    Json::Value result;
    result["status"] = "ok";
    co_return drogon::HttpResponse::newHttpJsonResponse(result);
}

drogon::Task<drogon::HttpResponsePtr>
    UserController::remove_user(drogon::HttpRequestPtr req) {
}

drogon::Task<drogon::HttpResponsePtr>
    UserController::get_users(drogon::HttpRequestPtr req) {
    const auto users = co_await m_service.get_users();
    Json::Value result;
    for (const auto& user : users) {
        Json::Value user_json;

        user_json["name"] = user.name;

        result.append(user_json);
    }

    co_return drogon::HttpResponse::newHttpJsonResponse(result);
}

} // namespace bxt::UI
