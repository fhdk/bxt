/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "Package.h"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cctype>
#include <fmt/format.h>
#include <stdexcept>
#include <string>
#include <vector>

namespace bxt::Core::Domain {

Package Package::from_filename(const Section& section,
                               const std::string& filename) {
    std::vector<std::string> substrings;

    boost::split(substrings, filename, boost::is_any_of("-"));

    auto subsize = substrings.size();

    if (subsize < 4) {
        throw std::invalid_argument("Invalid package filename");
    }

    auto release = substrings[subsize - 2];
    auto version_substr = substrings[subsize - 3];

    auto version_pos = filename.find(version_substr);
    auto name = filename.substr(0, version_pos - 1);

    std::string epoch = "0";
    auto version = version_substr;

    auto epoch_pos = version_substr.find(":");

    if (epoch_pos != std::string::npos) {
        epoch = version_substr.substr(0, epoch_pos);
        version = version_substr.substr(epoch_pos + 1, version_substr.size());
    }

    static const auto digit_validator = [](const char& ch) {
        return std::isdigit(ch);
    };

    auto valid_epoch =
        std::ranges::all_of(epoch.begin(), epoch.end(), digit_validator);
    auto valid_release =
        std::ranges::all_of(release.begin(), release.end(), digit_validator);

    auto valid_name =
        std::ranges::all_of(name.begin(), name.end(),
                            [](const char& ch) { return std::isalnum(ch); });

    if (!(valid_epoch && valid_release && valid_name)) {
        throw std::invalid_argument("Invalid package filename");
    }

    std::optional<int> epoch_int;
    try {
        epoch_int = std::stoi(epoch);
    } catch ([[maybe_unused]] const std::invalid_argument& art) {
        epoch_int = std::nullopt;
    }
    return Package(section, name,
                   {.epoch = epoch_int, .version = version, .release = release},
                   PackageArchitecture(), filename);
}

Package Package::from_filepath(const Section& section,
                               const std::filesystem::path& filepath) {
    auto result = from_filename(section, filepath.filename());
    result.set_filepath(filepath);
    return result;
}

} // namespace bxt::Core::Domain
