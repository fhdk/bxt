/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#pragma once

#include "utilities/alpmdb/Desc.h"
#include "utilities/box/PoolManager.h"

#include <boost/serialization/optional.hpp>

namespace bxt::Box {
struct Package {
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& name;
        ar& filepath;
        ar& signature_path;
        ar& location;
        ar& description;
    }
    std::string name;
    std::filesystem::path filepath;
    boost::optional<std::filesystem::path> signature_path = {};
    PoolManager::PoolLocation location;
    Utilities::AlpmDb::Desc description;
};
} // namespace bxt::Box