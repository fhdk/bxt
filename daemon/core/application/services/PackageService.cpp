/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "PackageService.h"

namespace bxt::Core::Application {

coro::task<std::vector<Package>>
    PackageService::get_packages(const PackageSectionDTO& section_dto) const {
    auto section = SectionDTOMapper::to_entity(section_dto);

    return m_repository.find_by_section_async(section);
}

} // namespace bxt::Core::Application
