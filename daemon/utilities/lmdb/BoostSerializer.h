/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: %YEAR% Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/dtos/UserDTO.h"
#include "core/domain/entities/PackageLogEntry.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/set.hpp>

template<class Archive>
void boost::serialization::serialize(Archive &ar,
                                     bxt::Core::Application::UserDTO &user,
                                     const unsigned int version)
{
    ar &user.name;
    ar &user.password;
    ar &user.permissions;
}

template<class Archive>
void boost::serialization::load(Archive &ar,
                                bxt::Core::Domain::PackageLogEntry &entry,
                                const unsigned int version)
{
    using namespace std::chrono;
    int64_t time;
    ar &time;
    entry.set_time(time_point<system_clock>(microseconds(time)));

    bxt::Core::Domain::LogEntryType type;
    ar &type;
    entry.set_type(type);
}

template<class Archive>
void boost::serialization::save(Archive &ar,
                                const bxt::Core::Domain::PackageLogEntry &entry,
                                const unsigned int version)
{
    using namespace std::chrono;
    int64_t time = duration_cast<microseconds>(entry.time().time_since_epoch()).count();
    ar &time;

    bxt::Core::Domain::LogEntryType type = entry.type();
    ar &type;
}
BOOST_SERIALIZATION_SPLIT_FREE(bxt::Core::Domain::PackageLogEntry)

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
