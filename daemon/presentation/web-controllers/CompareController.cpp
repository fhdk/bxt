/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "CompareController.h"

#include "core/application/dtos/PackageSectionDTO.h"
#include "drogon/HttpResponse.h"

#include "json/value.h"
#include <vector>
namespace bxt::Presentation {
drogon::Task<drogon::HttpResponsePtr>
    CompareController::compare(drogon::HttpRequestPtr req) {
    const auto sections_json = *req->getJsonObject();

    std::vector<PackageSectionDTO> sections;

    for (const auto& section_json : sections_json) {
        sections.emplace_back(PackageSectionDTO {
            .branch = section_json["branch"].asString(),
            .repository = section_json["repository"].asString(),
            .architecture = section_json["architecture"].asString()});
    }

    const auto compare_result = co_await m_compare_service.compare(sections);

    Json::Value result;

    for (const auto& section : compare_result->sections) {
        Json::Value section_json;

        section_json["branch"] = section.branch;
        section_json["repository"] = section.repository;
        section_json["architecture"] = section.architecture;

        result["sections"].append(section_json);
    }

    for (const auto& comparison : compare_result->compare_table) {
        result["compare_table"][comparison.first.first]
              [std::string(comparison.first.second)] = comparison.second;
    }

    co_return drogon::HttpResponse::newHttpJsonResponse(result);
}
} // namespace bxt::Presentation