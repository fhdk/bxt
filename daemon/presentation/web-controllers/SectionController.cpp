/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "SectionController.h"

#include <drogon/HttpResponse.h>
#include <json/value.h>

drogon::Task<drogon::HttpResponsePtr>
    bxt::Presentation::SectionController::get_sections(
        drogon::HttpRequestPtr req) const {
    const auto sections = co_await m_service.get_sections();

    Json::Value result;

    for (const auto& section : sections) {
        Json::Value section_json;

        section_json["branch"] = section.branch;
        section_json["repo"] = section.repository;
        section_json["architecture"] = section.architecture;

        result.append(section_json);
    }

    co_return drogon::HttpResponse::newHttpJsonResponse(result);
}
