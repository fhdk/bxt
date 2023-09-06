/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "utilities/Error.h"

#include <archive.h>
#include <exception>
#include <string>

namespace Archive {

struct InvalidEntryError : public bxt::Error {
    const std::string message() const noexcept override {
        return "The entry has no linked archive!";
    }
};

struct LibArchiveError : public bxt::Error {
    explicit LibArchiveError(archive* archive) {
        error_string = archive_error_string(archive);
    }

    const std::string message() const noexcept override { return error_string; }

private:
    std::string error_string;
};

} // namespace Archive
