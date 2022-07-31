/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once
#include "utilities/libarchive/Reader.h"
#include "utilities/libarchive/Writer.h"
namespace bxt::Utilities::AlpmDb {

class Database {
public:
    Database() = default;

    void repo_add(const std::vector<std::filesystem::path>& files);

private:
    Archive::Writer m_writer;
};

} // namespace bxt::Utilities::AlpmDb
