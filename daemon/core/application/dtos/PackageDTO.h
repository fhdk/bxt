/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "PackageSectionDTO.h"

#include <filesystem>
#include <string>

namespace bxt::Core::Application {
struct PackageDTO {
    PackageSectionDTO section;
    std::string name;
    std::filesystem::path filepath;

    auto operator<=>(const PackageDTO& other) const = default;
};
} // namespace bxt::Core::Application
