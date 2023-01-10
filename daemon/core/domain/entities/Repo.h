/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/entities/AggregateRoot.h"
#include "core/domain/entities/Architecture.h"
#include "core/domain/value_objects/Name.h"

namespace bxt::Core::Domain
{

class Repo : public AggregateRoot<Name>
{
public:
    const std::set<Architecture> &architectures() const { return m_architectures; }

    void set_architectures(const std::set<Architecture> &new_architectures)
    {
        m_architectures = new_architectures;
    }

private:
    std::set<Architecture> m_architectures;
};

} // namespace bxt::Core::Domain
