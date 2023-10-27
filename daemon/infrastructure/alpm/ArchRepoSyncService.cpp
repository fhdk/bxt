/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "ArchRepoSyncService.h"

#include "boost/uuid/name_generator.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "core/application/events/IntegrationEventBase.h"
#include "core/application/events/SyncEvent.h"
#include "coro/sync_wait.hpp"
#include "coro/when_all.hpp"
#include "httplib.h"
#include "utilities/alpmdb/Desc.h"
#include "utilities/libarchive/Reader.h"

#include <coro/thread_pool.hpp>
#include <initializer_list>

namespace bxt::Infrastructure {

coro::task<void> ArchRepoSyncService::sync(const PackageSectionDTO& section) {
    if (!m_options.sources.contains(section)) { co_return; }
    const auto remote_packages = co_await get_available_packages(section);

    std::vector<coro::task<PackageFile>> tasks;

    const auto uuid = boost::uuids::random_generator()();

    auto task = [this, section,
                 uuid](const auto& pkgname) -> coro::task<PackageFile> {
        co_return co_await download_package(section, pkgname, uuid);
    };

    for (const auto& pkgname : remote_packages) {
        tasks.emplace_back(task(pkgname));
    }

    const auto files = co_await coro::when_all(std::move(tasks));

    for (const auto& file_task : files) {
        const auto file = file_task.return_value();
        auto package_result = Package::from_filepath(
            SectionDTOMapper::to_entity(file.section()), file.file_path());
        package_result->set_pool_location(Box::PoolManager::PoolLocation::Sync);

        if (package_result.has_value()) {
            co_await m_package_repository.add_async(*package_result);
        }
    }

    co_await m_package_repository.commit_async();

    co_return;
}

coro::task<void> ArchRepoSyncService::sync_all() {
    std::vector<coro::task<void>> tasks;
    for (const auto& src : m_options.sources) {
        tasks.emplace_back(sync(src.first));
    }

    const auto event = std::make_shared<Core::Application::Events::SyncEvent>();

    std::initializer_list<Core::Application::Events::IntegrationEventPtr>
        event_list {event};

    event->started = true;
    co_await m_dispatcher.dispatch_async(event_list);

    co_await coro::when_all(std::move(tasks));

    event->started = false;
    co_await m_dispatcher.dispatch_async(event_list);

    co_return;
}

coro::task<std::vector<std::string>>
    ArchRepoSyncService::get_available_packages(
        const PackageSectionDTO& section) {
    std::vector<std::string> result;

    const auto client =
        co_await get_client(m_options.sources[section].repo_url);

    const auto repository_name =
        m_options.sources[section].repo_name.value_or(section.repository);

    const auto db_path_format =
        fmt::format("{}/{{repository}}.db",
                    m_options.sources[section].repo_structure_template);

    const auto path = fmt::format(
        fmt::runtime(db_path_format), fmt::arg("branch", section.branch),
        fmt::arg("repository", repository_name),
        fmt::arg("architecture", section.architecture));

    auto response = client->Get(
        path, httplib::Headers(), [](uint64_t current, uint64_t total) {
            std::cout << fmt::format("Downloading database... {}/{}\n", current,
                                     total);
            return true;
        });

    if (response.error() != httplib::Error::Success) { co_return {}; }

    if (response->status != 200) { co_return result; }

    Archive::Reader reader;

    archive_read_support_filter_all(reader);
    archive_read_support_format_all(reader);

    reader.open_memory(reinterpret_cast<uint8_t*>(response->body.data()),
                       response->body.size());

    for (auto& [header, entry] : reader) {
        std::string pname = archive_entry_pathname(*header);

        if (!pname.ends_with("/desc")) { continue; }

        auto contents = entry.read_all();

        if (!contents.has_value()) { continue; }

        Utilities::AlpmDb::Desc desc(reinterpret_cast<char*>(contents->data()));

        const auto filename = desc.get("FILENAME");
        if (!filename.has_value()) { continue; }
        result.emplace_back(*filename);
    }

    co_return result;
}

coro::task<PackageFile>
    ArchRepoSyncService::download_package(const PackageSectionDTO& section,
                                          const std::string& package_filename,
                                          const boost::uuids::uuid& id) {
    auto client = co_await get_client(m_options.sources[section].repo_url);

    auto repository_name =
        m_options.sources[section].repo_name.value_or(section.repository);

    auto path_format = fmt::format(
        "{}/{{pkgfname}}", m_options.sources[section].repo_structure_template);

    auto path = fmt::format(fmt::runtime(path_format),
                            fmt::arg("branch", section.branch),
                            fmt::arg("repository", repository_name),
                            fmt::arg("architecture", section.architecture),
                            fmt::arg("pkgfname", package_filename));

    auto filepath = m_options.sources[section].download_path
                    / std::string(boost::uuids::to_string(id));

    std::filesystem::create_directories(filepath);

    auto full_filename =
        fmt::format("{}/{}", filepath.string(), package_filename);

    if (std::filesystem::exists(full_filename)) {
        co_return PackageFile(section, full_filename);
    }

    std::cout << fmt::format("Downloading {} as {}...\n", package_filename,
                             full_filename);

    std::ofstream stream(full_filename);

    auto response =
        client->Get(path, [&](const char* data, size_t data_length) {
            stream.write(data, data_length);
            return true;
        });

    co_return PackageFile(section, full_filename);
}

coro::task<std::unique_ptr<httplib::SSLClient>>
    ArchRepoSyncService::get_client(const std::string& url) {
    co_await tp.schedule();

    auto client_ptr = std::make_unique<httplib::SSLClient>(url);

    client_ptr->set_follow_location(true);
    client_ptr->enable_server_certificate_verification(true);

    co_return client_ptr;
}

} // namespace bxt::Infrastructure
