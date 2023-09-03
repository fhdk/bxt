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
#include <filesystem>
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

    static const auto release_tag_validator = [](const char& ch) {
        return std::isalnum(ch) || ch == '.';
    };

    auto valid_epoch =
        std::ranges::all_of(epoch.begin(), epoch.end(), digit_validator);

    auto valid_version = std::ranges::all_of(release.begin(), release.end(),
                                             release_tag_validator);

    auto valid_release = std::ranges::all_of(release.begin(), release.end(),
                                             release_tag_validator);

    auto valid_name =
        std::ranges::all_of(name.begin(), name.end(), [](const char& ch) {
            static std::string valid = "@._+-";
            return (std::isalpha(ch) && std::islower(ch)) || std::isdigit(ch)
                   || (valid.find(ch) != std::string::npos);
        });

    valid_name = valid_name && !name.starts_with("-") && !name.starts_with(".");

    if (!(valid_epoch && valid_release && valid_name && valid_version)) {
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

    const auto signature_path = fmt::format("{}.sig", filepath.string());

    if (std::filesystem::exists(signature_path)) {
        result.set_signature_path(signature_path);
    }
    return result;
}

} // namespace bxt::Core::Domain
