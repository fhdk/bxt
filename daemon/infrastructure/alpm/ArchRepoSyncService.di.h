/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include "ArchRepoSyncService.h"

#include <core/application/services/SyncService.di.h>
#include <core/domain/repositories/PackageRepositoryBase.di.h>
#include <kangaru/service.hpp>
#pragma once

namespace bxt::Infrastructure::di {

struct ArchRepoSyncService
    : kgr::single_service<
          bxt::Infrastructure::ArchRepoSyncService,
          kgr::dependency<bxt::Core::Domain::di::PackageRepositoryBase,
                          kgr::mapped_service_t<ArchRepoOptions>>>,
      kgr::overrides<bxt::Core::Application::di::SyncService> {};

} // namespace bxt::Infrastructure::di
