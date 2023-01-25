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
    static ArchRepoSource from_node(const YAML::Node& node) {
        if (!node.IsDefined()) { return ArchRepoSource {}; }

        ArchRepoSource result;
        auto get_if_defined =
            [node]<typename TReturn>(const std::string& value,
                                     const TReturn& default_value) -> TReturn {
            if (!node[value].IsDefined()) { return default_value; }
            return node[value].as<TReturn>();
        };

        result.repo_url = get_if_defined("repo-url", result.repo_url);
        result.repo_structure_template = get_if_defined(
            "repo-structure-template", result.repo_structure_template);
        result.pool_path = get_if_defined("pool-path", result.pool_path);

        return result;
    };

    std::string repo_url = "cloudflaremirrors.com";
    std::string repo_structure_template =
        "/archlinux/{repository}/os/{architecture}";
    std::string pool_path = "box/pool/sync";

    std::optional<std::string> repo_name;
};

} // namespace bxt::Infrastructure
