/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#pragma once

#include "core/application/dtos/PackageSectionDTO.h"
#include "core/domain/entities/Section.h"
#include "core/domain/repositories/RepositoryBase.h"
#include "coro/task.hpp"

#include <vector>

namespace bxt::Core::Application {

class SectionService {
public:
    SectionService(Domain::ReadOnlyRepositoryBase<Domain::Section>& repository)
        : m_repository(repository) {}

    coro::task<std::vector<PackageSectionDTO>> get_sections() const;

private:
    Domain::ReadOnlyRepositoryBase<Domain::Section>& m_repository;
};

} // namespace bxt::Core::Application