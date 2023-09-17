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
nonstd::expected<Package, Package::ParsingError>
    Package::from_filename(const Section& section,
                           const std::string& filename) {
    std::vector<std::string> substrings;

    boost::split(substrings, filename, boost::is_any_of("-"));

    auto subsize = substrings.size();

    if (subsize < 4) {
        return nonstd::make_unexpected(Package::ParsingError(
            Package::ParsingError::ErrorCode::InvalidFilename));
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
        Package::ParsingError::ErrorCode error_code =
            Package::ParsingError::ErrorCode::InvalidFilename;
        if (!valid_epoch) {
            error_code = Package::ParsingError::ErrorCode::InvalidEpoch;
        } else if (!valid_version) {
            error_code = Package::ParsingError::ErrorCode::InvalidVersion;
        } else if (!valid_release) {
            error_code = Package::ParsingError::ErrorCode::InvalidReleaseTag;
        } else if (!valid_name) {
            error_code = Package::ParsingError::ErrorCode::InvalidName;
        }
        return nonstd::make_unexpected(Package::ParsingError(error_code));
    }

    std::optional<int> epoch_int;
    try {
        epoch_int = std::stoi(epoch);
    } catch (const std::invalid_argument& art) { epoch_int = std::nullopt; }
    return Package(section, name,
                   {.epoch = epoch_int, .version = version, .release = release},
                   PackageArchitecture(), filename);
}

nonstd::expected<Package, Package::ParsingError> Package::from_filepath(
    const Section& section,
    const std::filesystem::path& filepath,
    const std::optional<std::filesystem::path>& signature_path) {
    auto result = from_filename(section, filepath.filename());

    if (!result.has_value()) { return result; }

    result->set_filepath(filepath);

    if (signature_path.has_value()) {
        result->set_signature_path(signature_path);
        return result;
    }

    const auto deduced_signature_path =
        fmt::format("{}.sig", filepath.string());

    if (std::filesystem::exists(deduced_signature_path)) {
        result->set_signature_path(deduced_signature_path);
    }
    return result;
}

} // namespace bxt::Core::Domain
