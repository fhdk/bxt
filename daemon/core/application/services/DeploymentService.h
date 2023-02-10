/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/dtos/PackageDTO.h"

#include <coro/task.hpp>

namespace bxt::Core::Application {

class DeploymentService {
public:
    virtual ~DeploymentService() = default;

    virtual coro::task<void> deploy(PackageDTO package) = 0;

    virtual coro::task<bool> verify_token(const std::string& token) = 0;
};

} // namespace bxt::Core::Application
