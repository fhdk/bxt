/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "Database.h"

namespace bxt::Utilities::AlpmDb {

void Database::repo_add(const std::vector<std::filesystem::path>& files) {
    for (const auto& file : files) {
        try {
            Archive::Reader file_reader;
            file_reader.open_filename(file);

            for (auto& [a, b] : file_reader) {
                if (!a) { b.skip(); }
            }
        }
    }
}

} // namespace bxt::Utilities::AlpmDb
