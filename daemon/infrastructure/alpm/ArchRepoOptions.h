/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#pragma once

#include "utilities/configuration/Configuration.h"

namespace bxt::Infrastructure {

struct ArchRepoOptions : public Utilities::Configurable {
    std::string repo_url = "cloudflaremirrors.com";
    std::string repo_structure_template =
        "/archlinux/{repository}/os/{architecture}";

    virtual void serialize(Utilities::Configuration &config) override {
        config.set("repo-url", repo_url);
        config.set("repo-name", repo_structure_template);
    }
    virtual void deserialize(const Utilities::Configuration &config) override {
        repo_url = config.get<std::string>("repo-url").value_or(repo_url);
        repo_structure_template =
            config.get<std::string>("repo-structure-template")
                .value_or(repo_structure_template);
    }
};

} // namespace bxt::Infrastructure
