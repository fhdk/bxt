/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "Name.h"

#include <stdexcept>

namespace bxt::Core::Domain {
Name::Name(const std::string &name_string) : m_name_string(name_string) {
  if (name_string.empty()) {
    throw new std::invalid_argument("Name cannot be empty");
  }
}

} // namespace bxt::Core::Domain
