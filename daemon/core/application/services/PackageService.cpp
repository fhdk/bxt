/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "PackageService.h"

#include "core/application/dtos/PackageDTO.h"
#include "core/application/dtos/PackageSectionDTO.h"

#include <algorithm>
#include <bits/ranges_algo.h>
#include <iterator>
#include <vector>

namespace bxt::Core::Application {

coro::task<std::vector<PackageDTO>>
    PackageService::get_packages(const PackageSectionDTO section_dto) const {
    auto section = SectionDTOMapper::to_entity(section_dto);

    auto result_entities = co_await m_repository.find_by_section_async(section);

    std::vector<PackageDTO> result;
    result.reserve(result_entities.size());
    std::ranges::transform(result_entities, std::back_inserter(result),
                           PackageDTOMapper::to_dto);

    co_return result;
}

} // namespace bxt::Core::Application
