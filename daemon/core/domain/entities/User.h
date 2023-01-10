/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/entities/AggregateRoot.h"
#include "core/domain/entities/Permission.h"
#include "core/domain/value_objects/Name.h"

#include <set>
#include <string>

namespace bxt::Core::Domain
{

class User : public AggregateRoot<>
{
public:
    const Name &name() const { return m_name; }

    const std::string &password() const { return m_password; }

    const std::set<Permission> &permissions() const { return m_permissions; }

    void set_name(const std::string &new_name) { m_name = new_name; }

    void set_password(const std::string &new_password) { m_password = new_password; }

    void set_permissions(const std::set<Permission> &new_permissions)
    {
        m_permissions = new_permissions;
    }

private:
    Name m_name;
    std::string m_password;
    std::set<Permission> m_permissions;
};

} // namespace bxt::Core::Domain
