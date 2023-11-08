/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "Package.h"

#include "core/domain/value_objects/PackageVersion.h"
#include "fmt/core.h"
#include "scn/tuple_return/tuple_return.h"
#include "utilities/Error.h"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cctype>
#include <filesystem>
#include <fmt/format.h>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

bool check_valid_name(std::string_view name) {
    if (name.empty()) { return false; }

    static constexpr std::string_view valid = "@._+-";
    return name[0] != '-' && name[0] != '.'
           && std::ranges::all_of(name, [](const char& ch) {
                  return (std::isalpha(ch) && std::islower(ch))
                         || std::isdigit(ch)
                         || (valid.find(ch) != std::string::npos);
              });
}

namespace bxt::Core::Domain {
Package::ParseResult Package::from_filename(const Section& section,
                                            const std::string& filename) {
    std::vector<std::string> substrings;

    boost::split(substrings, filename, boost::is_any_of("-"));

    auto subsize = substrings.size();

    if (subsize < 4) {
        return bxt::make_error<ParsingError>(
            ParsingError::ErrorCode::InvalidFilename);
    }

    auto release_substr = substrings[subsize - 2];
    auto version_substr = substrings[subsize - 3];

    auto version_pos = filename.find(version_substr);
    auto name = filename.substr(0, version_pos - 1);

    auto valid_name = check_valid_name(name);

    if (!valid_name) {
        return bxt::make_error<ParsingError>(
            ParsingError::ErrorCode::InvalidName);
    }

    auto version = PackageVersion::from_string(
        fmt::format("{}-{}", version_substr, release_substr));

    if (!version.has_value()) {
        return bxt::make_error_with_source<ParsingError>(
            std::move(version.error()),
            ParsingError::ErrorCode::InvalidVersion);
    }

    return Package(section, name, *version, PackageArchitecture(), filename,
                   Box::PoolManager::PoolLocation::Unknown);
}

Package::ParseResult Package::from_filepath(
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
