/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "PermissionController.h"

namespace bxt::Presentation {
drogon::Task<drogon::HttpResponsePtr>
    PermissionController::add_permission(drogon::HttpRequestPtr req) {
    auto json = *req->getJsonObject();
    const auto user_id = json["user_id"].asString();
    const auto permission_str = json["permission"].asString();
    co_await m_service.add(user_id, permission_str);

    Json::Value result;
    result["status"] = "ok";
    co_return drogon::HttpResponse::newHttpJsonResponse(result);
}

drogon::Task<drogon::HttpResponsePtr>
    PermissionController::remove_permission(drogon::HttpRequestPtr req) {
    auto json = *req->getJsonObject();
    const auto user_id = json["user_id"].asString();
    const auto permission_str = json["permission"].asString();

    co_await m_service.remove(user_id, permission_str);

    Json::Value result;
    result["status"] = "ok";
    co_return drogon::HttpResponse::newHttpJsonResponse(result);
}

drogon::Task<drogon::HttpResponsePtr>
    PermissionController::get_permissions(drogon::HttpRequestPtr req) {
    const auto user_id = req->getParameter("user_id");

    auto permissions = co_await m_service.get(user_id);

    Json::Value result;
    result["permissions"] = Json::Value(Json::arrayValue);
    for (const auto& permission : permissions) {
        result["permissions"].append(permission);
    }
    co_return drogon::HttpResponse::newHttpJsonResponse(result);
}

} // namespace bxt::Presentation
