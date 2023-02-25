/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include "di.h"

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <drogon/HttpAppFramework.h>
#include <kangaru/debug.hpp>

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
    using namespace bxt;

    ctr.invoke<di::Utilities::RepoSchema::Parser,
               di::Infrastructure::ArchRepoOptions>(
        [](bxt::Utilities::RepoSchema::Parser& parser,
           bxt::Infrastructure::ArchRepoOptions& options) {
            parser.extend(&options);

            parser.parse("./box.yml");
        });

    ctr.invoke<di::Utilities::LMDB::Environment>(
        [](std::shared_ptr<bxt::Utilities::LMDB::Environment> lmdbenv) {
            lmdbenv->env().set_mapsize(1UL * 1024UL * 1024UL * 1024UL);
            lmdbenv->env().set_max_dbs(10);

            std::filesystem::create_directories("./bxtd.mdb/");

            lmdbenv->env().open("./bxtd.mdb/", 0, 0664);
        });

    ctr.service<di::Persistence::SectionRepository>();
    ctr.service<di::Persistence::UserRepository>();
    ctr.service<di::Persistence::Box>();

    ctr.service<di::Core::Application::AuthService>();
    ctr.service<di::Core::Application::PackageService>();
    ctr.service<di::Core::Application::PermissionService>();

    ctr.service<di::Infrastructure::DeploymentService>();
    ctr.service<di::Infrastructure::ArchRepoSyncService>();
}

void setup_controllers(auto& app, kgr::container& ctr) {
    using namespace bxt::di::UI;

    app.registerController(ctr.service<PackageController>())
        .registerController(ctr.service<AuthController>())
        .registerController(ctr.service<UserController>())
        .registerController(ctr.service<PermissionController>())
}

int main() {
    setup_logger();

    kgr::container ctr;

    setup_di_container(ctr);

    auto& app = drogon::app()
                    .addListener("0.0.0.0", 8080)
                    .setUploadPath("/tmp/bxt/")
                    .setClientMaxBodySize(256 * 1024 * 1024)
                    .setClientMaxMemoryBodySize(1024 * 1024);

    setup_controllers(app, ctr);

    app.run();

    return 0;
}
