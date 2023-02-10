/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/services/AuthService.h"

#include <core/domain/repositories/UserRepository.di.h>
#include <kangaru/service.hpp>

namespace bxt::Core::Application::di {
struct AuthService
    : kgr::single_service<
          bxt::Core::Application::AuthService,
          kgr::dependency<bxt::Core::Domain::di::UserRepository>> {};
} // namespace bxt::Core::Application::di