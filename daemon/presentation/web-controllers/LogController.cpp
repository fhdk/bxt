/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "LogController.h"

#include "drogon/HttpTypes.h"

namespace bxt::Presentation {
drogon::Task<drogon::HttpResponsePtr>
    LogController::get_package_logs(drogon::HttpRequestPtr req) {
    Json::Value result;

    auto dtos = co_await m_service.events();

    if (dtos.empty()) {
        result["status"] = "error";
        result["error"] = "No events found";

        auto response = drogon::HttpResponse::newHttpJsonResponse(result);
        response->setStatusCode(drogon::k400BadRequest);

        co_return response;
    }

    for (const auto& dto : dtos) {
        Json::Value json_value;

        json_value["id"] = dto.id;
        json_value["time"] = dto.time;
        json_value["package"]["name"] = dto.package.name;

        // Serialize section
        json_value["package"]["section"]["branch"] = dto.package.section.branch;
        json_value["package"]["section"]["repository"] =
            dto.package.section.repository;
        json_value["package"]["section"]["architecture"] =
            dto.package.section.architecture;

        // Serialize pool_entries
        Json::Value pool_entries_json;
        for (const auto& [pool_location, pool_entry] :
             dto.package.pool_entries) {
            Json::Value entry_json;
            entry_json["version"] = pool_entry.version;
            entry_json["hasSignature"] = pool_entry.signature_path.has_value();

            pool_entries_json.append(entry_json);
        }

        json_value["package"]["pool_entries"] = pool_entries_json;
        const auto preferred_candidate =
            Box::PoolManager::select_preferred_value(dto.package.pool_entries);

        json_value["package"]["preferredCandidate"]["version"] =
            preferred_candidate->version;
        json_value["package"]["preferredCandidate"]["hasSignature"] =
            preferred_candidate->signature_path.has_value() ? "true" : "false";

        // Serialize action
        json_value["action"] = [dto] {
            switch (dto.type) {
            case Core::Domain::Add: return "Add";
            case Core::Domain::Remove: return "Remove";
            case Core::Domain::Update: return "Update";
            }
        }();

        result.append(json_value);
    }

    co_return drogon::HttpResponse::newHttpJsonResponse(result);
}

} // namespace bxt::Presentation
