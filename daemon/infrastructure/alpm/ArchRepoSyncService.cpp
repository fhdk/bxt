/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "ArchRepoSyncService.h"

#include "coro/sync_wait.hpp"
#include "coro/when_all.hpp"
#include "utilities/alpmdb/Desc.h"
#include "utilities/libarchive/Reader.h"

#include <coro/thread_pool.hpp>

namespace bxt::Infrastructure {

coro::task<void> ArchRepoSyncService::sync(const PackageSectionDTO& section) {
    const auto remote_packages = co_await get_available_packages(section);
    coro::thread_pool tp {coro::thread_pool::options {.thread_count = 4}};

    std::vector<coro::task<PackageFile>> tasks;

    auto task = [this,
                 section](const auto& pkgname) -> coro::task<PackageFile> {
        //            co_await tp.schedule();

        co_return co_await download_package(section, pkgname);
    };

    for (const auto& pkgname : remote_packages) {
        tasks.emplace_back(task(pkgname));
    }

    auto files = coro::sync_wait(coro::when_all(std::move(tasks)));
}

coro::task<std::vector<std::string>>
    ArchRepoSyncService::get_available_packages(
        const PackageSectionDTO& section) {
    std::vector<std::string> result;

    auto db_path_format =
        fmt::format("{}/{{repository}}.db", options.repo_structure_template);

    auto path = fmt::format(fmt::runtime(db_path_format),
                            fmt::arg("branch", section.branch),
                            fmt::arg("repository", section.repository),
                            fmt::arg("architecture", section.architecture));

    auto response = m_client.Get(
        path, httplib::Headers(), [](uint64_t current, uint64_t total) {
            std::cout << fmt::format("Downloading database... {}/{}\n", current,
                                     total);
            return true;
        });

    if (response->status != 200) { co_return result; }

    Archive::Reader reader;

    archive_read_support_filter_all(reader);
    archive_read_support_format_all(reader);

    reader.open_memory(reinterpret_cast<uint8_t*>(response->body.data()),
                       response->body.size());

    for (auto& [header, entry] : reader) {
        std::string pname = archive_entry_pathname(*header);

        if (!pname.ends_with("/desc")) { continue; }

        auto data = entry.read_all();

        Utilities::AlpmDb::Desc desc(reinterpret_cast<char*>(data.data()));
        try {
            result.emplace_back(desc.get("FILENAME"));
        } catch (const std::invalid_argument& arg) {}
    }

    co_return result;
}

coro::task<PackageFile>
    ArchRepoSyncService::download_package(const PackageSectionDTO& section,
                                          const std::string& package_filename) {
    auto path_format =
        fmt::format("{}/{{pkgfname}}", options.repo_structure_template);

    auto path = fmt::format(fmt::runtime(path_format),
                            fmt::arg("branch", section.branch),
                            fmt::arg("repository", section.repository),
                            fmt::arg("architecture", section.architecture),
                            fmt::arg("pkgfname", package_filename));

    auto filepath = fmt::format("{}/bxtd/{}",
                                std::filesystem::temp_directory_path().string(),
                                package_filename);

    std::cout << fmt::format("Downloading {} as {}...\n", package_filename,
                             filepath);

    std::ofstream stream(filepath);

    auto response =
        m_client.Get(path, [&](const char* data, size_t data_length) {
            stream.write(data, data_length);
            return true;
        });

    co_return PackageFile(section, filepath);
}

} // namespace bxt::Infrastructure
