/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/repositories/SectionReadOnlyRepository.di.h"
#include "persistence/config/SectionRepository.h"

#include <kangaru/kangaru.hpp>

namespace bxt::Persistence::di {
struct SectionRepository
    : kgr::single_service<bxt::Persistence::SectionRepository, kgr::autowire>,
      kgr::overrides<Core::Domain::di::SectionReadOnlyRepositoryBase> {};
} // namespace bxt::Persistence::di
