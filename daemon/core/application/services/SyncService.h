/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/dtos/PackageSectionDTO.h"

#include <coro/task.hpp>

namespace bxt::Core::Application {

class SyncService {
public:
    //    SyncService(PackageRepositoryBase& repository) :
    //    m_repository(repository) {}
    virtual ~SyncService() = default;

    virtual coro::task<void> sync(const PackageSectionDTO& section) = 0;

private:
    //    PackageRepositoryBase& m_repository;
};

} // namespace bxt::Core::Application
