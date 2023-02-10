/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include "core/domain/repositories/UserRepository.di.h"
#include "persistence/lmdb/UserRepository.h"

#include <kangaru/service.hpp>
#pragma once

namespace bxt::Persistence::di {
struct UserRepository
    : kgr::single_service<bxt::Persistence::UserRepository, kgr::autowire>,
      kgr::overrides<bxt::Core::Domain::di::UserRepository> {};
} // namespace bxt::Persistence::di
