/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "DeploymentService.h"

#include "infrastructure/PackageService.h"

#include <filesystem>
#include <fmt/format.h>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

namespace bxt::Infrastructure {

coro::task<uint64_t> DeploymentService::deploy_start() {
    std::mt19937_64 engine(std::random_device {}());
    std::uniform_int_distribution<uint64_t> distribution;
    auto session_id = distribution(engine);

    m_session_packages.try_emplace(session_id, std::vector<PackageDTO>());

    co_return session_id;
}

coro::task<void> DeploymentService::deploy_push(PackageDTO package,
                                                uint64_t session_id) {
    if (!std::filesystem::exists(package.filepath)) {
        throw std::invalid_argument("File not found");
    }
    if (package.signature_path) {
        if (!std::filesystem::exists(*package.signature_path)) {
            throw std::invalid_argument("Invalid signature file");
        }
    }

    m_session_packages.at(session_id).emplace_back(package);

    co_return;
}

coro::task<bool> DeploymentService::verify_session(uint64_t session_id) {
    co_return m_session_packages.count(session_id) > 0;
}

coro::task<void> DeploymentService::deploy_end(uint64_t session_id) {
    const auto &packages = m_session_packages.at(session_id);
    PackageService::Transaction transaction;
    transaction.to_add = packages;

    co_await m_package_service.commit_transaction(transaction);

    m_session_packages.erase(session_id);

    co_return;
}

} // namespace bxt::Infrastructure
