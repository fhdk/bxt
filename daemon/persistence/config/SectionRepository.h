/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/entities/Section.h"
#include "core/domain/helpers/RepositoryBase.h"
#include "core/domain/value_objects/Name.h"
#include "utilities/repo-schema/Parser.h"

#include <yaml-cpp/yaml.h>

namespace bxt::Persistence {

class SectionRepository
    : public Core::Domain::ReadOnlyRepositoryBase<bxt::Core::Domain::Section,
                                                  bxt::Core::Domain::Name> {
public:
    SectionRepository(Utilities::RepoSchema::Parser &parser)
        : m_parser(parser) {}

    virtual bxt::Core::Domain::Section
        find_by_id(const bxt::Core::Domain::Name &id) override;
    virtual bxt::Core::Domain::Section find_first(
        std::function<bool(const bxt::Core::Domain::Section &)>) override;
    virtual std::vector<bxt::Core::Domain::Section>
        find(std::function<bool(const bxt::Core::Domain::Section &)>) override;
    virtual coro::task<std::vector<bxt::Core::Domain::Section>>
        all_async() override;

private:
    Utilities::RepoSchema::Parser &m_parser;
};

} // namespace bxt::Persistence
