/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/services/SyncService.h"

#include <kangaru/kangaru.hpp>

namespace bxt::Core::Application::di {

struct SyncService
    : kgr::abstract_service<bxt::Core::Application::SyncService> {};

} // namespace bxt::Core::Application::di
