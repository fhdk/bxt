/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/services/AuthService.h"
#include "core/application/services/PackageService.h"
#include "core/application/services/PermissionService.h"
#include "core/application/services/UserService.h"
#include "infrastructure/DeploymentService.h"
#include "infrastructure/alpm/ArchRepoOptions.h"
#include "infrastructure/alpm/ArchRepoSyncService.h"
#include "persistence/config/SectionRepository.h"
#include "persistence/lmdb/UserRepository.h"
#include "ui/web-controllers/AuthController.h"
#include "ui/web-controllers/PackageController.h"
#include "ui/web-controllers/PermissionController.h"
#include "ui/web-controllers/UserController.h"
#include "ui/web-filters/JwtFilter.h"
#include "utilities/lmdb/Environment.h"
#include "utilities/repo-schema/Parser.h"

#include <kangaru/debug.hpp>
#include <kangaru/kangaru.hpp>

namespace bxt::di {

namespace Utilities {

    namespace LMDB {

        struct Environment
            : kgr::shared_service<bxt::Utilities::LMDB::Environment> {};

    } // namespace LMDB

    namespace RepoSchema {

        struct Parser
            : kgr::autowire_single_service<bxt::Utilities::RepoSchema::Parser> {
        };

    } // namespace RepoSchema

} // namespace Utilities

namespace Core {

    namespace Domain {

        struct UserRepository
            : kgr::abstract_service<bxt::Core::Domain::UserRepository> {};

        struct PackageRepositoryBase
            : kgr::abstract_service<bxt::Core::Domain::PackageRepositoryBase> {
        };

        struct ReadOnlySectionRepository
            : kgr::abstract_service<
                  bxt::Core::Domain::ReadOnlyRepositoryBase<Section>> {};

        struct PermissionMatcher
            : kgr::single_service<bxt::Core::Domain::PermissionMatcher> {};

    } // namespace Domain

    namespace Application {

        struct AuthService
            : kgr::single_service<
                  bxt::Core::Application::AuthService,
                  kgr::dependency<di::Core::Domain::UserRepository>> {};

        struct DeploymentService
            : kgr::abstract_service<bxt::Core::Application::DeploymentService> {
        };

        struct PackageService
            : kgr::single_service<
                  bxt::Core::Application::PackageService,
                  kgr::dependency<di::Core::Domain::PackageRepositoryBase>> {};

        struct SyncService
            : kgr::abstract_service<bxt::Core::Application::SyncService> {};

        struct UserService
            : kgr::single_service<
                  bxt::Core::Application::UserService,
                  kgr::dependency<di::Core::Domain::UserRepository>> {};

        struct PermissionService
            : kgr::single_service<
                  bxt::Core::Application::PermissionService,
                  kgr::dependency<di::Core::Domain::UserRepository,
                                  di::Core::Domain::PermissionMatcher>> {};

    } // namespace Application

} // namespace Core

namespace Persistence {

    struct UserRepository
        : kgr::single_service<
              bxt::Persistence::UserRepository,
              kgr::dependency<di::Utilities::LMDB::Environment>>,
          kgr::overrides<di::Core::Domain::UserRepository> {};

    struct Box
        : kgr::single_service<
              bxt::Persistence::PackageRepository,
              kgr::dependency<di::Core::Domain::ReadOnlySectionRepository>>,
          kgr::overrides<di::Core::Domain::PackageRepositoryBase> {};

    struct SectionRepository
        : kgr::single_service<
              bxt::Persistence::SectionRepository,
              kgr::dependency<di::Utilities::RepoSchema::Parser>>,
          kgr::overrides<di::Core::Domain::ReadOnlySectionRepository> {};

} // namespace Persistence

namespace Infrastructure {

    struct DeploymentService
        : kgr::single_service<
              bxt::Infrastructure::DeploymentService,
              kgr::dependency<di::Core::Domain::PackageRepositoryBase>>,
          kgr::overrides<di::Core::Application::DeploymentService> {};

    struct ArchRepoOptions
        : kgr::single_service<bxt::Infrastructure::ArchRepoOptions> {};

    struct ArchRepoSyncService
        : kgr::single_service<
              bxt::Infrastructure::ArchRepoSyncService,
              kgr::dependency<di::Core::Domain::PackageRepositoryBase,
                              di::Infrastructure::ArchRepoOptions>>,
          kgr::overrides<di::Core::Application::SyncService> {};

} // namespace Infrastructure

namespace UI {

    struct PackageController
        : kgr::shared_service<
              bxt::UI::PackageController,
              kgr::dependency<di::Core::Application::DeploymentService,
                              di::Core::Application::PackageService>> {};

    struct AuthController
        : kgr::shared_service<
              bxt::UI::AuthController,
              kgr::dependency<di::Core::Application::AuthService>> {};

    struct UserController
        : kgr::shared_service<
              bxt::UI::UserController,
              kgr::dependency<di::Core::Application::UserService>> {};

    struct PermissionController
        : kgr::shared_service<
              bxt::UI::PermissionController,
              kgr::dependency<di::Core::Application::PermissionService>> {};

    struct JwtFilter
        : kgr::shared_service<
              bxt::UI::JwtFilter,
              kgr::dependency<di::Core::Application::AuthService>> {};

} // namespace UI

} // namespace bxt::di
