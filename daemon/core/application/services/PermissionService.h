/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/application/dtos/UserDTO.h"
#include "core/domain/repositories/UserRepository.h"
#include "core/domain/services/PermissionMatcher.h"
#include "core/domain/value_objects/Permission.h"

#include <string>
#include <string_view>

namespace bxt::Core::Application {
class PermissionService {
public:
    PermissionService(Core::Domain::UserRepository& repository,
                      Core::Domain::PermissionMatcher& matcher)
        : m_repository(repository), m_matcher(matcher) {}

    coro::task<void> add(const std::string& user_name,
                         const std::string& permission);
    coro::task<void> remove(const std::string& user_name,
                            const std::string& permission);

    coro::task<std::vector<std::string>> get(const std::string& user_name);

    coro::task<bool> check(const std::string_view permission,
                           const std::string& user_name);

private:
    Core::Domain::UserRepository& m_repository;
    Core::Domain::PermissionMatcher& m_matcher;
};

} // namespace bxt::Core::Application
