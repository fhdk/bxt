/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/services/PackageService.h"

#include <kangaru/kangaru.hpp>

namespace bxt::Core::Application::di {

struct PackageService
    : kgr::single_service<bxt::Core::Application::PackageService,
                          kgr::autowire> {};

} // namespace bxt::Core::Application::di
