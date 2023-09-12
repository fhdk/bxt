/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#pragma once

#include "lmdbxx/lmdb++.h"
#include "utilities/errors/DatabaseError.h"

#include <string>
namespace bxt::Utilities::LMDB {

struct Error : public bxt::Error {
    Error(const lmdb::error&& error) : error(std::move(error)) {}

    const std::string message() const noexcept override { return error.what(); }

private:
    lmdb::error error;
};

struct SerializationError : public bxt::Error {
    SerializationError() = default;

    const std::string message() const noexcept override {
        return "Serialization/deserialization error";
    }
};

} // namespace bxt::Utilities::LMDB