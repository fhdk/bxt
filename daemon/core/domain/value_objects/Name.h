/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include <string>

namespace bxt::Core::Domain
{

class Name
{
public:
    Name(const std::string& name_string);

    operator const std::string&() const { return m_name_string; }

private:
    std::string m_name_string;
};

} // namespace bxt::Core::Domain
