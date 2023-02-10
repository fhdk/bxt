/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "DeploymentServiceOptions.h"
#include "core/application/dtos/PackageDTO.h"
#include "core/application/services/DeploymentService.h"
#include "core/domain/repositories/PackageRepositoryBase.h"
#include "utilities/Mapper.h"

namespace bxt::Infrastructure {

class DeploymentService : public bxt::Core::Application::DeploymentService {
public:
    DeploymentService(PackageRepositoryBase& repository)
        : m_repository(repository) {}

    virtual coro::task<void> deploy(PackageDTO package) override;
    virtual coro::task<bool> verify_token(const std::string& token) override;

private:
    DeploymentServiceOptions m_options;
    PackageRepositoryBase& m_repository;
    Utilities::Mapper<Package, PackageDTO> m_dto_mapper;
};

} // namespace bxt::Infrastructure
