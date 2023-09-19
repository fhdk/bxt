/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "Section.h"
#include "core/domain/entities/AggregateRoot.h"
#include "core/domain/value_objects/Name.h"
#include "core/domain/value_objects/PackageArchitecture.h"
#include "core/domain/value_objects/PackageVersion.h"
#include "frozen/map.h"
#include "utilities/Error.h"

#include <filesystem>
#include <nonstd/expected.hpp>
#include <optional>
#include <string>

namespace bxt::Core::Domain {

class Package {
public:
    struct TId {
        Section section;
        Name package_name;
    };

    const TId id() const { return {m_section, m_name}; }
    const std::string& name() const { return m_name; }
    const PackageVersion& version() const { return m_version; }
    const std::string& architecture() const { return m_architecture; }
    const std::filesystem::path& filepath() const { return m_filepath; }

    Package(const Section& section,
            const std::string& name,
            const PackageVersion& version,
            const PackageArchitecture& arch,
            const std::filesystem::path& path)
        : m_section(section),
          m_name(name),
          m_version(version),
          m_architecture(arch),
          m_filepath(path) {}

    virtual ~Package() = default;

    std::string string() const {
        return fmt::format("{}-{}-{}", name(), version().string(),
                           architecture());
    }

    struct ParsingError : public bxt::Error {
        enum class ErrorCode {
            InvalidFilename,
            InvalidVersion,
            InvalidReleaseTag,
            InvalidName,
            InvalidEpoch
        };

        const ErrorCode error_code;

        static inline const frozen::map<ErrorCode, std::string_view, 5>
            error_messages = {
                {ErrorCode::InvalidFilename, "Invalid package filename"},
                {ErrorCode::InvalidVersion, "Invalid package version"},
                {ErrorCode::InvalidReleaseTag, "Invalid package release tag"},
                {ErrorCode::InvalidName, "Invalid package name"},
                {ErrorCode::InvalidEpoch, "Invalid package epoch"}};

        ParsingError(ErrorCode error_code) : error_code(error_code) {
            message = error_messages.at(error_code).data();
        }
    };
    using ParseResult = nonstd::expected<Package, ParsingError>;

    static ParseResult from_filename(const Section& section,
                                     const std::string& filename);
    static ParseResult from_filepath(
        const Section& section,
        const std::filesystem::path& filepath,
        const std::optional<std::filesystem::path>& signature_path = {});

    Section section() const { return m_section; }

    void set_name(const std::string& new_name) { m_name = new_name; }

    void set_version(const PackageVersion& new_version) {
        m_version = new_version;
    }

    void set_architecture(const std::string& new_architecture) {
        m_architecture = new_architecture;
    }

    void set_filepath(const std::filesystem::path& new_filepath) {
        m_filepath = new_filepath;
    }

    void set_section(const Section& new_section) { m_section = new_section; }

    std::optional<std::filesystem::path> signature_path() const {
        return m_signature_path;
    }

    void set_signature_path(
        const std::optional<std::filesystem::path>& signature_path) {
        m_signature_path = signature_path;
    }

private:
    Section m_section;

    Name m_name;
    PackageVersion m_version;
    PackageArchitecture m_architecture;
    std::filesystem::path m_filepath;
    std::optional<std::filesystem::path> m_signature_path = {};
};

} // namespace bxt::Core::Domain
