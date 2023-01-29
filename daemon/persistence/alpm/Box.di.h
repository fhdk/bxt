/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#pragma once

#include "Box.h"
#include "core/domain/repositories/PackageRepositoryBase.di.h"
#include "core/domain/repositories/SectionReadOnlyRepository.di.h"

#include <kangaru/service.hpp>

namespace bxt::Persistence::di {
struct Box
    : kgr::single_service<
          bxt::Persistence::Box,
          kgr::dependency<Core::Domain::di::SectionReadOnlyRepositoryBase>>,
      kgr::overrides<Core::Domain::di::PackageRepositoryBase> {};
} // namespace bxt::Persistence::di
