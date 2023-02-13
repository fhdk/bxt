/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/services/DeploymentService.di.h"
#include "core/application/services/DeploymentService.h"

#include <drogon/drogon.h>
#include <kangaru/autowire.hpp>

namespace bxt::UI {

class PackageController
    : public drogon::HttpController<PackageController, false> {
public:
    PackageController(Core::Application::DeploymentService &service)
        : m_service(service) {};

public:
    METHOD_LIST_BEGIN

    ADD_METHOD_TO(PackageController::deploy, "/deploy", drogon::Post);

    METHOD_LIST_END

    drogon::Task<drogon::HttpResponsePtr> deploy(drogon::HttpRequestPtr req);

private:
    Core::Application::DeploymentService &m_service;
};

} // namespace bxt::UI
