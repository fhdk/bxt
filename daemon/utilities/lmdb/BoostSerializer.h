/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: %YEAR% Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/dtos/PackageDTO.h"
#include "core/application/dtos/PackageLogEntryDTO.h"
#include "core/application/dtos/PackageSectionDTO.h"
#include "core/application/dtos/UserDTO.h"
#include "core/domain/entities/PackageLogEntry.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/set.hpp>

template<class Archive>
void boost::serialization::serialize(Archive &ar,
                                     std::filesystem::path &path,
                                     const unsigned int version) {
    std::string path_string = path.string();
    ar &path_string;
    if (Archive::is_loading::value) {
        path = std::filesystem::path(path_string);
    }
}

template<class Archive>
void boost::serialization::serialize(
    Archive &ar,
    bxt::Core::Application::PackageSectionDTO &section,
    const unsigned int version) {
    ar &section.branch;
    ar &section.repository;
    ar &section.architecture;
}

template<class Archive>
void boost::serialization::serialize(Archive &ar,
                                     bxt::Core::Application::UserDTO &user,
                                     const unsigned int version) {
    ar &user.name;
    ar &user.password;
    ar &user.permissions;
}

template<class Archive>
void boost::serialization::serialize(Archive &ar,
                                     bxt::Core::Application::PackageDTO &pkg,
                                     const unsigned int version) {
    ar &pkg.name;
    ar &pkg.filepath;
    ar &pkg.section;
}

template<class Archive>
void boost::serialization::serialize(
    Archive &ar,
    bxt::Core::Application::PackageLogEntryDTO &entry,
    const unsigned int version) {
    ar &entry.id;
    ar &entry.package;
    ar &entry.time;
    ar &entry.type;
}

namespace bxt::Utilities::LMDB {
template<typename T> struct BoostSerializer {
    static std::string serialize(const T &value) {
        std::stringstream entity_stream;
        boost::archive::text_oarchive entity_archive(entity_stream);
        entity_archive << value;

        return entity_stream.str();
    }

    static T deserialize(std::string_view value) {
        std::stringstream entity_stream((std::string(value)));
        boost::archive::text_iarchive entity_archive(entity_stream);

        T result;
        entity_archive >> result;

        return result;
    }
};
} // namespace bxt::Utilities::LMDB
