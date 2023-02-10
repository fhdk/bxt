/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include <coro/mutex.hpp>
namespace bxt::Utilities {
template<typename T> struct locked {
    coro::scoped_lock lock;

    T value;
};
} // namespace bxt::Utilities
