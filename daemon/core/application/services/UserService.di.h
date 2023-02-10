/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/services/UserService.h"
#include "core/domain/repositories/UserRepository.di.h"

#include <kangaru/service.hpp>
namespace bxt::Core::Application::di {

struct UserService
    : kgr::single_service<
          bxt::Core::Application::UserService,
          kgr::dependency<bxt::Core::Domain::di::UserRepository>> {};

} // namespace bxt::Core::Application::di
