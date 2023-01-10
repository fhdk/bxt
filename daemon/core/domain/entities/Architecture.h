/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/entities/AggregateRoot.h"
#include "core/domain/value_objects/Name.h"

#include <set>
#include <string>

namespace bxt::Core::Domain
{
class Architecture : public AggregateRoot<Name>
{
public:
    const std::set<Name> &ignored_package_names() const { return m_ignored_package_names; }

    void set_ignored_package_names(const std::set<Name> &new_ignored_package_names)
    {
        m_ignored_package_names = new_ignored_package_names;
    }

private:
    std::set<Name> m_ignored_package_names;
};
} // namespace bxt::Core::Domain
