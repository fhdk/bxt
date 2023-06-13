/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/services/DeploymentService.h"
#include "core/application/services/PackageService.h"
#include "utilities/drogon/Macros.h"

#include <drogon/drogon.h>
#include <kangaru/autowire.hpp>

namespace bxt::Presentation {

class PackageController
    : public drogon::HttpController<PackageController, false> {
public:
    PackageController(Core::Application::DeploymentService &service,
                      Core::Application::PackageService &package_service)
        : m_service(service), m_package_service(package_service) {};

    METHOD_LIST_BEGIN

    BXT_ADD_METHOD_TO(PackageController::deploy_start, "/api/deploy/start", drogon::Post);
    BXT_ADD_METHOD_TO(PackageController::deploy_push, "/api/deploy/push", drogon::Post);
    BXT_ADD_METHOD_TO(PackageController::deploy_end, "/api/deploy/end", drogon::Post);

    BXT_ADD_METHOD_TO(PackageController::sync, "/api/sync", drogon::Post);

    METHOD_LIST_END

    drogon::Task<drogon::HttpResponsePtr> deploy_start(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> deploy_push(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> deploy_end(drogon::HttpRequestPtr req);

    drogon::Task<drogon::HttpResponsePtr> sync(drogon::HttpRequestPtr req);

private:
    std::string m_key = "KEY0";
    Core::Application::DeploymentService &m_service;
    Core::Application::PackageService &m_package_service;
};

} // namespace bxt::Presentation
