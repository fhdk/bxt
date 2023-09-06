/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "frozen/unordered_map.h"
#include "tl/expected.hpp"
#include "utilities/Error.h"
#include "utilities/alpmdb/PkgInfo.h"
#include "utilities/errors/Macro.h"

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace bxt::Utilities::AlpmDb {

class Desc {
public:
    struct ParseError : public bxt::Error {
        enum class ErrorType {
            InvalidArchive,
            NoPackageInfo,
        };

        ParseError(ErrorType type) : error_type(type) {}
        explicit ParseError(ErrorType type, const bxt::Error&& source)
            : bxt::Error(std::make_unique<bxt::Error>(std::move(source))),
              error_type(type) {}

        const std::string message() const noexcept override {
            return error_messages.at(error_type).data();
        }

    private:
        ErrorType error_type;

        // Define the error messages map
        static constexpr frozen::unordered_map<ErrorType, std::string_view, 2>
            error_messages = {
                {ErrorType::InvalidArchive, "This file is not a valid archive"},
                {ErrorType::NoPackageInfo, "No package info"},
            };
    };
    BXT_DECLARE_RESULT(ParseError)

    Desc() = default;
    explicit Desc(const std::string& contents, const std::string& files = "");

    static Result<Desc> parse_package(const std::filesystem::path& filepath);

    std::optional<std::string> get(const std::string& key) const;

    std::string string() const;
    std::string files() const;

private:
    std::string m_desc;
    std::string m_files;
};
} // namespace bxt::Utilities::AlpmDb
