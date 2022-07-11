/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once

#include "core/domain/entities/AggregateRoot.h"

namespace bxt::Core::Domain
{
class Permission : public AggregateRoot<>
{
public:
    Permission();

private:
};

} // namespace bxt::Core::Domain
