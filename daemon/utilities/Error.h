/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once
#include "fmt/format.h"

#include <memory>
#include <stdexcept>
#include <string>

namespace bxt {

struct Error {
    Error() = default;
    explicit Error(std::unique_ptr<bxt::Error>&& source)
        : source(std::move(source)) {}

    Error(Error&& other) noexcept : source(std::move(other.source)) {}
    Error(const Error& other) {
        if (!other.source) { return; }
        source = std::make_unique<bxt::Error>(*other.source);
    }
    Error& operator=(const Error& other) {
        if (!other.source) { return *this; }
        this->source = std::make_unique<bxt::Error>(*other.source);
        return *this;
    }
    Error& operator=(Error&& other) noexcept {
        this->source = std::make_unique<bxt::Error>(std::move(other));
        return *this;
    }

    virtual ~Error() = default;

    std::unique_ptr<bxt::Error> source = nullptr;

    virtual const std::string message() const noexcept {
        return "Unknown error";
    }

    const std::string what() const noexcept {
        auto result = message();
        if (source) { result += fmt::format("\n\nFrom:\n{}", source->what()); }
        return result;
    }
};

} // namespace bxt