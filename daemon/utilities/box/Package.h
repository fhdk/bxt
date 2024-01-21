/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#pragma once

#include "utilities/alpmdb/Desc.h"
#include "utilities/box/PoolManager.h"

#define USE_CEREAL 1
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <filesystem>
#include <parallel_hashmap/phmap.h>

namespace bxt::Box {

struct PackageDescription {
    std::filesystem::path filepath;
    std::optional<std::filesystem::path> signature_path = {};
    Utilities::AlpmDb::Desc descfile;

    template<typename Archive> void serialize(Archive& ar) {
        ar(filepath, signature_path, descfile);
    }
};

struct Package {
    template<class Archive> void serialize(Archive& ar) {
        ar(name, descriptions);
    }
    std::string name;
    bool is_any_architecture;
    phmap::flat_hash_map<PoolManager::PoolLocation, PackageDescription>
        descriptions;
};
} // namespace bxt::Box