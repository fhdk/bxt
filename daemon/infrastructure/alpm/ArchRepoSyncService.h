/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "ArchRepoOptions.h"
#include "core/application/services/SyncService.h"
#include "infrastructure/PackageFile.h"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>

namespace bxt::Infrastructure {

class ArchRepoSyncService : public bxt::Core::Application::SyncService {
public:
    ArchRepoSyncService() {
        m_client.set_follow_location(true);
        m_client.enable_server_certificate_verification(true);
    }

    virtual coro::task<void> sync(const PackageSectionDTO& section) override;

protected:
    coro::task<std::vector<std::string>>
        get_available_packages(const PackageSectionDTO& section);
    coro::task<PackageFile> download_package(const PackageSectionDTO& section,
                                             const std::string& name);

private:
    ArchRepoOptions options;
    httplib::SSLClient m_client = httplib::SSLClient(options.repo_url);
};

} // namespace bxt::Infrastructure
