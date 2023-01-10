/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/repositories/PackageRepositoryBase.h"

namespace bxt::Utilities {

template<typename TTo, typename TFrom> struct Mapper {
    TTo map(const TFrom& from);
};

} // namespace bxt::Utilities
