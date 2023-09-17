/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "frozen/map.h"
#include "utilities/Error.h"

namespace bxt {

class DatabaseError : public bxt::Error {
public:
    enum class ErrorType {
        IOError,
        DatabaseMalformedError,
        InvalidEntityError,
        EntityNotFound
    };
    DatabaseError(ErrorType error_type) : error_type(error_type) {}

    DatabaseError(ErrorType error_type, const bxt::Error&& source)
        : bxt::Error(std::make_unique<bxt::Error>(std::move(source))),
          error_type(error_type) {}

    static inline frozen::map<ErrorType, std::string_view, 4> messages {
        {ErrorType::IOError, "IO error"},
        {ErrorType::DatabaseMalformedError, "Database is malformed"},
        {ErrorType::InvalidEntityError, "Invalid entity"},
        {ErrorType::EntityNotFound, "Entity not found"}};

    const std::string message() const noexcept override {
        return messages.at(error_type).data();
    }

private:
    ErrorType error_type;
};

} // namespace bxt