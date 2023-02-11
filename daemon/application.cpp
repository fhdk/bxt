/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "core/application/services/AuthService.di.h"
#include "infrastructure/DeploymentService.di.h"
#include "infrastructure/alpm/ArchRepoSyncService.di.h"
#include "persistence/alpm/Box.di.h"
#include "persistence/config/SectionRepository.di.h"
#include "ui/web-controllers/AuthController.h"
#include "ui/web-controllers/PackageController.h"
#include "ui/web-controllers/UserController.h"
#include "utilities/repo-schema/Parser.h"

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <drogon/HttpAppFramework.h>
#include <kangaru/debug.hpp>
#include <persistence/lmdb/UserRepository.di.h>
#include <utilities/lmdb/Environment.h>

void setup_logger() {
    using namespace boost::log;

    add_common_attributes();
    add_console_log(std::cout,
                    boost::log::keywords::format =
                        "[%TimeStamp%][%Severity%]: %Message%",
                    boost::log::keywords::auto_flush = true);
    add_file_log("journal.log",
                 boost::log::keywords::format =
                     "[%TimeStamp%][%Severity%]: %Message%",
                 boost::log::keywords::auto_flush = true);
}

void setup_di_container(kgr::container& ctr) {
    ctr.invoke([](bxt::Utilities::RepoSchema::Parser& parser,
                  bxt::Infrastructure::ArchRepoOptions& options) {
        parser.extend(&options);

        parser.parse("./box.yml");
    });

    ctr.invoke([](bxt::Utilities::LMDB::Environment& lmdbenv) {
        lmdbenv.env().set_mapsize(1UL * 1024UL * 1024UL * 1024UL);
        lmdbenv.env().set_max_dbs(10);

        std::filesystem::create_directories("./bxtd.mdb/");

        lmdbenv.env().open("./bxtd.mdb/", 0, 0664);
    });

    ctr.service<bxt::Persistence::di::SectionRepository>();
    ctr.service<bxt::Persistence::di::UserRepository>();
    ctr.service<bxt::Core::Application::di::AuthService>();

    ctr.service<bxt::Persistence::di::Box>();
    ctr.service<bxt::Infrastructure::di::DeploymentService>();

    ctr.service<bxt::Infrastructure::di::ArchRepoSyncService>();
}

int main() {
    setup_logger();

    kgr::container ctr;

    setup_di_container(ctr);

    drogon::app()
        .addListener("0.0.0.0", 8080)
        .setUploadPath("/tmp/bxt/")
        .setClientMaxBodySize(256 * 1024 * 1024)
        .setClientMaxMemoryBodySize(1024 * 1024)
        .registerController(ctr.service<PackageControllerService>())
        .registerController(ctr.service<AuthControllerService>())
        .registerController(ctr.service<UserControllerService>())

        .run();

    return 0;
}
