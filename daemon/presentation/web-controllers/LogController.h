/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#pragma once
#include <drogon/HttpController.h>

namespace bxt::Presentation {

class LogController : public drogon::HttpController<LogController, false> {
public:
    LogController();
};

} // namespace bxt::Presentation
