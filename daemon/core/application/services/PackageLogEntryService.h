/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: %YEAR% Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "dexode/EventBus.hpp"

#include <memory>

#include "core/domain/entities/PackageLogEntry.h"
#include "core/domain/repositories/RepositoryBase.h"

namespace bxt::Core::Application {

class PackageLogEntryService
{
public:
    PackageLogEntryService(
        std::shared_ptr<dexode::EventBus> evbus,
        bxt::Core::Domain::ReadWriteRepositoryBase<Domain::PackageLogEntry> &repository)
        : m_evbus(evbus), m_repository(repository)
    {
        init();
    }

    void init();

private:
    std::shared_ptr<dexode::EventBus> m_evbus;
    bxt::Core::Domain::ReadWriteRepositoryBase<Domain::PackageLogEntry> &m_repository;
};

} // namespace bxt::Core::Application
