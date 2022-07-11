/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "PackageService.h"

namespace bxt::Core::Domain {

PackageService::PackageService() {
}

ArchitectureMatch
    PackageService::match_architectures(const PackageArchitecture &arch,
                                        const Architecture &arch_section) {
  if (std::string(arch) == "any") { return ArchitectureMatch::Any; }

  if (std::string(arch_section.id()) == std::string(arch)) {
    return ArchitectureMatch::Matches;
  } else {
    return ArchitectureMatch::NotMatches;
  }
}

} // namespace bxt::Core::Domain
