/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "DeploymentService.h"
#include "core/application/services/DeploymentService.di.h"
#include "core/domain/repositories/PackageRepositoryBase.di.h"

#include <kangaru/service.hpp>

namespace bxt::Infrastructure::di {

struct DeploymentService
    : kgr::single_service<
          bxt::Infrastructure::DeploymentService,
          kgr::dependency<bxt::Core::Domain::di::PackageRepositoryBase>>,
      kgr::overrides<bxt::Core::Application::di::DeploymentService> {};

} // namespace bxt::Infrastructure::di
