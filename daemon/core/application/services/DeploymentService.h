/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/dtos/PackageDTO.h"

#include <coro/task.hpp>
#include <filesystem>

namespace bxt::Core::Application {

class DeploymentService {
public:
    virtual ~DeploymentService() = default;

    virtual coro::task<uint64_t> deploy_start() = 0;
    virtual coro::task<void> deploy_push(PackageDTO package,
                                         uint64_t session_id) = 0;
    virtual coro::task<void> deploy_end(uint64_t session_id) = 0;

    virtual coro::task<bool> verify_session(uint64_t session_id) = 0;
};

} // namespace bxt::Core::Application
