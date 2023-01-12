/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "utilities/repo-schema/SchemaExtension.h"

#include <optional>

namespace bxt::Infrastructure {
struct ArchRepoSource {
    std::string repo_url = "cloudflaremirrors.com";
    std::string repo_structure_template =
        "/archlinux/{repository}/os/{architecture}";
    std::optional<std::string> repo_name;
};

} // namespace bxt::Infrastructure
