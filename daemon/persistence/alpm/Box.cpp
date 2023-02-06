/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "Box.h"

#include "coro/sync_wait.hpp"

#include <infrastructure/PackageFile.h>

namespace bxt::Persistence {

coro::task<Box::TResult> Box::find_by_id_async(TId id) {
}

coro::task<Box::TResult>
    Box::find_first_async(std::function<bool(const Package &)>) {
}

coro::task<Box::TResults>
    Box::find_async(std::function<bool(const Package &)> condition) {
}

coro::task<Box::TResults> Box::all_async() {
}

coro::task<void> Box::add_async(const Package entity) {
    auto section_dto = m_section_dto_mapper.map(entity.section());

    std::cout << fmt::format("Adding {}/{}\n", std::string(section_dto),
                             entity.filepath().filename().string());

    std::error_code ec;
    std::filesystem::create_symlink(
        entity.filepath(),
        fmt::format("{}/{}/{}", m_options.location, std::string(section_dto),
                    entity.filepath().filename().string()),
        ec);

    std::set<std::string> paths_to_add = {entity.filepath().string()};

    auto task = m_map.at(section_dto).add(paths_to_add);

    co_await task;

    co_return;
}

coro::task<void> Box::remove_async(const TId entity) {
    auto section_dto = m_section_dto_mapper.map(entity.section);

    std::filesystem::remove(
        fmt::format("{}/{}", m_options.location, std::string(section_dto)));

    std::set<std::string> names_to_remove = {entity.package_name};

    auto task = m_map.at(m_section_dto_mapper.map(entity.section))
                    .remove(names_to_remove);

    co_await task;

    co_return;
}

coro::task<void> Box::add_async(const std::vector<Package> entities) {
    phmap::node_hash_map<PackageSectionDTO, std::set<std::string>> paths_to_add;

    for (const auto &entity : entities) {
        auto section_dto = m_section_dto_mapper.map(entity.section());

        std::cout << fmt::format("Symlinking {}/{}\n", std::string(section_dto),
                                 entity.filepath().filename().string());

        std::error_code ec;

        auto target_path = std::filesystem::absolute(fmt::format(
            "{}/{}/{}", m_options.location, std::string(section_dto),
            entity.filepath().filename().string()));

        auto source_path = std::filesystem::relative(entity.filepath(),
                                                     target_path.parent_path());

        std::filesystem::create_symlink(source_path, target_path, ec);

        paths_to_add[section_dto].emplace(entity.filepath().string());
    }

    std::vector<coro::task<void>> add_tasks;
    add_tasks.reserve(paths_to_add.size());

    for (const auto &[section, values] : paths_to_add) {
        add_tasks.emplace_back(m_map.at(section).add(values));
    }

    co_await coro::when_all(std::move(add_tasks));

    co_return;
}

coro::task<void> Box::update_async(const Package entity) {
}

coro::task<std::vector<Core::Domain::Package>>
    Box::find_by_section_async(const Core::Domain::Section section) const {
    auto packages = m_map.at(m_section_dto_mapper.map(section)).packages();

    std::vector<Core::Domain::Package> result;
    result.reserve(packages.size());

    std::ranges::transform(
        packages, std::back_inserter(result), [section](const auto &package) {
            return Core::Domain::Package::from_filename(section, package);
        });

    co_return {result.begin(), result.end()};
}

} // namespace bxt::Persistence
