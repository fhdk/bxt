/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "SectionService.h"

#include "core/application/dtos/PackageSectionDTO.h"

#include <iterator>
#include <ranges>

coro::task<std::vector<PackageSectionDTO>>
    bxt::Core::Application::SectionService::get_sections() const {
    std::vector<PackageSectionDTO> result;

    const auto sections = co_await m_repository.all_async();

    if (!sections.has_value()) { co_return result; }

    result.reserve(sections->size());

    std::ranges::transform(*sections, std::back_inserter(result),
                           SectionDTOMapper::to_dto);

    co_return result;
}
