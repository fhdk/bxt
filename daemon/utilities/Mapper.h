/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

namespace bxt::Utilities {

template<typename TFrom, typename TTo> struct SerializerBase {
    virtual const TTo& map(const TFrom&) = 0;
};

} // namespace bxt::Utilities
