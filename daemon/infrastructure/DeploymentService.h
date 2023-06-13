/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include <cstdint>
#include <set>

#include "DeploymentServiceOptions.h"
#include "core/application/dtos/PackageDTO.h"
#include "core/application/services/DeploymentService.h"
#include "core/domain/repositories/PackageRepositoryBase.h"

#include "utilities/StaticDTOMapper.h"

namespace bxt::Infrastructure {

class DeploymentService : public bxt::Core::Application::DeploymentService {
public:
    DeploymentService(PackageRepositoryBase &repository) : m_repository(repository) {}

    virtual coro::task<uint64_t> deploy_start() override;
    virtual coro::task<void> deploy_push(PackageDTO package, uint64_t session_id) override;
    virtual coro::task<void> deploy_end(uint64_t session_id) override;

    virtual coro::task<bool> verify_session(uint64_t session_id) override;

private:
    coro::task<void> process_package(PackageDTO pkg);
    phmap::parallel_node_hash_map<uint64_t, std::set<PackageDTO>> m_session_packages;
    DeploymentServiceOptions m_options;
    PackageRepositoryBase& m_repository;
};

} // namespace bxt::Infrastructure
