/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#pragma once

#include "core/application/services/PermissionService.h"

#include <drogon/HttpController.h>

#define BXT_JWT_ADD_METHOD_TO(...) \
    ADD_METHOD_TO(__VA_ARGS__, "bxt::UI::JwtFilter")

namespace bxt::UI {

class PermissionController
    : public drogon::HttpController<PermissionController, false> {
public:
    METHOD_LIST_BEGIN

    // Add permission - requires admin authentication
    BXT_JWT_ADD_METHOD_TO(PermissionController::add_permission,
                          "/permissions/add",
                          drogon::Post);

    // Remove permission - requires admin authentication
    BXT_JWT_ADD_METHOD_TO(PermissionController::remove_permission,
                          "/permissions/remove",
                          drogon::Post);

    // Get all permissions - requires admin authentication
    BXT_JWT_ADD_METHOD_TO(PermissionController::get_permissions,
                          "/permissions",
                          drogon::Get);

    METHOD_LIST_END

    PermissionController(Core::Application::PermissionService& service)
        : m_service(service) {};

    drogon::Task<drogon::HttpResponsePtr>
        add_permission(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr>
        remove_permission(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr>
        get_permissions(drogon::HttpRequestPtr req);

private:
    Core::Application::PermissionService& m_service;
};

} // namespace bxt::UI
