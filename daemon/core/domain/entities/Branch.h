/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/entities/AggregateRoot.h"
#include "core/domain/entities/Repo.h"
#include "core/domain/value_objects/Name.h"

#include <set>
#include <string>

namespace bxt::Core::Domain
{

class Branch : public AggregateRoot<Name>
{
public:
    const std::set<Repo> &repos() const { return m_repos; }

protected:
    void set_repos(const std::set<Repo> &new_repos) { m_repos = new_repos; }

private:
    std::set<Repo> m_repos;
};

} // namespace bxt::Core::Domain
