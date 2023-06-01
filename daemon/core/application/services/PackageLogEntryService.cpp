/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: %YEAR% Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "PackageLogEntryService.h"

#include "core/domain/events/PackageEvents.h"

namespace bxt::Core::Application {

void PackageLogEntryService::init()
{
    auto listener = dexode::EventBus::Listener::createNotOwning(*m_evbus);

    listener.listen<bxt::Core::Domain::Events::PackageAdded>(
        [this](const Domain::Events::PackageAdded &added) {
            Domain::PackageLogEntry entry(added.package, Domain::LogEntryType::Add);

            m_repository.add(entry);
        });
    //    listener.listen<bxt::Core::Domain::Events::PackageRemoved>(
    //        [this](const Domain::Events::PackageRemoved &added) {
    //            Domain::PackageLogEntry entry(added.id, Domain::LogEntryType::Add);

    //            m_repository.add(entry);
    //        });
    listener.listen<bxt::Core::Domain::Events::PackageUpdated>(
        [this](const Domain::Events::PackageUpdated &added) {
            Domain::PackageLogEntry entry(added.new_package, Domain::LogEntryType::Add);

            m_repository.add(entry);
        });
}

} // namespace bxt::Core::Application
