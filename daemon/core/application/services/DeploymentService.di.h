/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/services/DeploymentService.h"

#include <kangaru/kangaru.hpp>

namespace bxt::Core::Application::di {

struct DeploymentService
    : kgr::abstract_service<bxt::Core::Application::DeploymentService> {};

} // namespace bxt::Core::Application::di
