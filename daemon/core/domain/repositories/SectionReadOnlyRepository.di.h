/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once
#include "core/domain/entities/Section.h"
#include "core/domain/helpers/RepositoryBase.h"

#include <kangaru/service.hpp>

namespace bxt::Core::Domain::di {
struct SectionReadOnlyRepositoryBase
    : kgr::abstract_service<ReadOnlyRepositoryBase<Section, Name>> {};
} // namespace bxt::Core::Domain::di
