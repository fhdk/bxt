/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/services/DeploymentService.h"
#include "core/application/services/PackageService.h"
#include "drogon/utils/FunctionTraits.h"
#include "drogon/utils/coroutine.h"
#include "utilities/drogon/Macros.h"

#include <drogon/drogon.h>
#include <kangaru/autowire.hpp>
#include <string>

namespace bxt::Presentation {

class PackageController
    : public drogon::HttpController<PackageController, false> {
public:
    PackageController(Core::Application::DeploymentService &service,
                      Core::Application::PackageService &package_service)
        : m_service(service), m_package_service(package_service) {};

    METHOD_LIST_BEGIN

    BXT_ADD_METHOD_TO(PackageController::deploy_start,
                      "/api/deploy/start",
                      drogon::Post);
    BXT_ADD_METHOD_TO(PackageController::deploy_push,
                      "/api/deploy/push",
                      drogon::Post);
    BXT_ADD_METHOD_TO(PackageController::deploy_end,
                      "/api/deploy/end",
                      drogon::Post);

    BXT_ADD_METHOD_TO(PackageController::add_package,
                      "/api/packages/add",
                      drogon::Post);
    BXT_ADD_METHOD_TO(PackageController::remove_package,
                      "/api/packages/remove",
                      drogon::Post);
    BXT_ADD_METHOD_TO(
        PackageController::get_packages,
        "/api/packages/get?branch={1}&repository={2}&architecture={3}",
        drogon::Get);

    BXT_ADD_METHOD_TO(PackageController::sync, "/api/sync", drogon::Post);

    METHOD_LIST_END

    drogon::Task<drogon::HttpResponsePtr>
        deploy_start(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr>
        deploy_push(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr>
        deploy_end(drogon::HttpRequestPtr req);

    drogon::Task<drogon::HttpResponsePtr> sync(drogon::HttpRequestPtr req);

    drogon::Task<drogon::HttpResponsePtr>
        add_package(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr>
        remove_package(drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr>
        get_packages(drogon::HttpRequestPtr req,
                     const std::string &branch,
                     const std::string &repository,
                     const std::string &architecture);

private:
    std::string m_key = "KEY0";
    Core::Application::DeploymentService &m_service;
    Core::Application::PackageService &m_package_service;
};

} // namespace bxt::Presentation
