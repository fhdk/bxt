/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/repositories/UserRepository.h"

#include <kangaru/service.hpp>

namespace bxt::Core::Domain::di {

struct UserRepository
    : kgr::abstract_service<bxt::Core::Domain::UserRepository> {};

} // namespace bxt::Core::Domain::di
