/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/repositories/PackageRepositoryBase.h"

#include <kangaru/service.hpp>

namespace bxt::Core::Domain::di {

struct PackageRepositoryBase
    : kgr::abstract_service<bxt::Core::Domain::PackageRepositoryBase> {};

} // namespace bxt::Core::Domain::di
