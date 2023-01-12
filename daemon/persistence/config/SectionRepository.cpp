/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "SectionRepository.h"

#include "core/domain/value_objects/Name.h"

namespace bxt::Persistence {

bxt::Core::Domain::Section bxt::Persistence::SectionRepository::find_by_id(
    const bxt::Core::Domain::Name& id) {
}

bxt::Core::Domain::Section bxt::Persistence::SectionRepository::find_first(
    std::function<bool(const bxt::Core::Domain::Section&)>) {
}

std::vector<bxt::Core::Domain::Section> SectionRepository::find(
    std::function<bool(const bxt::Core::Domain::Section&)>) {
}

coro::task<std::vector<Section>> SectionRepository::all_async() {
    const auto& sections = m_parser.sections();

    std::vector<Section> result;
    result.reserve(sections.size());

    std::ranges::transform(
        sections, std::back_inserter(result), [](const auto& section) {
            return Section(section.branch, section.repository,
                           section.architecture);
        });

    co_return {result.begin(), result.end()};
}

} // namespace bxt::Persistence
