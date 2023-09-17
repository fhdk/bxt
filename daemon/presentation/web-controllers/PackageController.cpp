/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "PackageController.h"

#include "boost/algorithm/string/classification.hpp"
#include "boost/algorithm/string/split.hpp"
#include "core/application/dtos/PackageDTO.h"
#include "core/application/dtos/PackageSectionDTO.h"
#include "core/application/services/PackageService.h"
#include "drogon/HttpResponse.h"
#include "drogon/HttpTypes.h"
#include "drogon/utils/FunctionTraits.h"
#include "jwt-cpp/traits/nlohmann-json/defaults.h"

#include "json/value.h"
#include <drogon/MultiPart.h>
#include <map>
#include <string>
#include <vector>

namespace bxt::Presentation {
using namespace drogon;

drogon::Task<HttpResponsePtr>
    PackageController::sync(drogon::HttpRequestPtr req) {
}

drogon::Task<drogon::HttpResponsePtr>
    PackageController::commit_transaction(drogon::HttpRequestPtr req) {
    MultiPartParser file_upload;
    file_upload.parse(req);

    const auto files_map = file_upload.getFilesMap();
    const auto params_map = file_upload.getParameters();

    std::map<int, PackageDTO> packages;

    for (const auto &[name, file] : files_map) {
        if (!name.starts_with("package")) { continue; }
        std::vector<std::string> parts;
        boost::split(parts, name, boost::is_any_of("."));

        if (parts.size() > 2) { continue; }

        auto file_number_str = parts[0].substr(7);
        const auto file_number = std::stoi(file_number_str);

        if (!packages.contains(file_number)) {
            packages.emplace(file_number, PackageDTO {});
        }

        file.save();

        if (parts.size() == 1 || parts[1] != "signature") {
            packages[file_number].filepath =
                app().getUploadPath() + "/" + file.getFileName();
        } else if (parts[1] == "signature") {
            packages[file_number].signature_path =
                app().getUploadPath() + "/" + file.getFileName();
        }
    }

    for (const auto &[name, param] : params_map) {
        if (!name.starts_with("package")) { continue; }

        std::vector<std::string> parts;
        boost::split(parts, name, boost::is_any_of("."));

        if (parts.size() != 2) { continue; }

        auto file_number_str = parts[0].substr(7);
        const auto file_number = std::stoi(file_number_str);

        if (!packages.contains(file_number)) { continue; }

        if (parts[1] == "branch") {
            packages[file_number].section.branch = param;
        } else if (parts[1] == "repository") {
            packages[file_number].section.repository = param;
        } else if (parts[1] == "architecture") {
            packages[file_number].section.architecture = param;
        }
    }

    PackageService::Transaction transaction;
    for (auto &package : packages) {
        transaction.to_add.emplace_back(std::move(package.second));
    }

    auto result = co_await m_package_service.commit_transaction(transaction);

    Json::Value res_json;
    res_json["result"] = result ? "ok" : "error";

    auto response = drogon::HttpResponse::newHttpJsonResponse(res_json);

    co_return response;
}

drogon::Task<drogon::HttpResponsePtr>
    PackageController::get_packages(drogon::HttpRequestPtr req,
                                    const std::string &branch,
                                    const std::string &repository,
                                    const std::string &architecture) {
    PackageSectionDTO section {branch, repository, architecture};
    Json::Value result;

    const auto packages = co_await m_package_service.get_packages(section);

    if (!packages.has_value()) {
        result["status"] = "error";
        result["message"] = packages.error().what();
        co_return drogon::HttpResponse::newHttpJsonResponse(result);
    }

    for (const auto &package : *packages) {
        Json::Value package_json;

        package_json["name"] = package.name;
        package_json["filename"] = package.filepath.filename().string();
        package_json["has_signature"] =
            package.signature_path.has_value() ? "true" : "false";

        result.append(package_json);
    }

    co_return drogon::HttpResponse::newHttpJsonResponse(result);
}

} // namespace bxt::Presentation
