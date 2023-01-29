/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "DeploymentService.h"

#include <iostream>

namespace bxt::Infrastructure {

coro::task<void> DeploymentService::deploy(PackageDTO package) {
    if (!std::filesystem::exists(package.filepath)) {
        throw std::invalid_argument("File not found");
    }

    std::filesystem::create_directories(m_options.pool(package.section));

    std::error_code ec;

    std::filesystem::rename(
        package.filepath,
        m_options.pool(package.section) / package.filepath.filename(), ec);

    // try copy + remove original
    if (ec) {
        std::filesystem::copy(
            package.filepath,
            m_options.pool(package.section) / package.filepath.filename(),
            std::filesystem::copy_options::overwrite_existing);

        std::filesystem::remove(package.filepath);
    }

    auto renamed_package = package;

    renamed_package.filepath =
        m_options.pool(package.section) / package.filepath.filename();

    co_await m_repository.add_async(m_dto_mapper.map(renamed_package));
    co_return;
}

} // namespace bxt::Infrastructure
