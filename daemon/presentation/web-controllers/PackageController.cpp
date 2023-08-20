/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "PackageController.h"

#include "core/application/dtos/PackageSectionDTO.h"
#include "drogon/utils/FunctionTraits.h"
#include "jwt-cpp/traits/nlohmann-json/defaults.h"

#include <drogon/MultiPart.h>
#include <string>

namespace bxt::Presentation {
using namespace drogon;

drogon::Task<drogon::HttpResponsePtr>
    PackageController::deploy_start(drogon::HttpRequestPtr req) {
    const auto key = req->headers().at("key");

    if (key != m_key) { co_return HttpResponse::newHttpResponse(); }

    const auto result = HttpResponse::newHttpResponse();
    result->setBody(std::to_string(co_await m_service.deploy_start()));

    co_return result;
}

drogon::Task<drogon::HttpResponsePtr>
    PackageController::deploy_push(drogon::HttpRequestPtr req) {
    MultiPartParser file_upload;

    int ok = file_upload.parse(req);
    if (ok < 0) { co_return HttpResponse::newHttpResponse(); }

    const auto headers = req->getHeaders();

    auto session_id = std::stoull(headers.at("session"));
    auto key = headers.at("key");

    if (key != m_key) { co_return HttpResponse::newHttpResponse(); }

    if (!co_await m_service.verify_session(session_id)) {
        co_return HttpResponse::newHttpResponse();
    }

    auto files_map = file_upload.getFilesMap();
    auto file = files_map.at("file");

    auto signature = files_map.at("signature");

    auto params_map = file_upload.getParameters();

    auto branch = params_map["branch"];
    auto repo = params_map["repository"];
    auto arch = params_map["architecture"];

    auto section = PackageSectionDTO {
        .branch = branch, .repository = repo, .architecture = arch};

    file.save();
    signature.save();

    auto name = file.getFileName();

    auto dto =
        PackageDTO {section, name, app().getUploadPath() + "/" + name, true};

    co_await m_service.deploy_push(
        dto, app().getUploadPath() + "/" + signature.getFileName(), session_id);

    auto resp = HttpResponse::newHttpResponse();

    resp->setExpiredTime(0);
    resp->setBody("ok");

    co_return resp;
}

drogon::Task<drogon::HttpResponsePtr>
    PackageController::deploy_end(drogon::HttpRequestPtr req) {
    const auto headers = req->getHeaders();
    auto session_id = headers.at("session");
    auto key = headers.at("key");

    co_await m_service.deploy_end(std::stoull(session_id));

    auto resp = HttpResponse::newHttpResponse();

    resp->setBody("ok");

    co_return resp;
}

drogon::Task<HttpResponsePtr>
    PackageController::sync(drogon::HttpRequestPtr req) {
}

drogon::Task<drogon::HttpResponsePtr>
    PackageController::add_package(drogon::HttpRequestPtr req) {
    /// TODO: Implement

    auto result = drogon::HttpResponse::newHttpResponse();
    result->setBody("Not implemented");
    co_return result;
}

drogon::Task<drogon::HttpResponsePtr>
    PackageController::remove_package(drogon::HttpRequestPtr req) {
    auto json = *req->getJsonObject();

    /// TODO: Implement

    auto result = drogon::HttpResponse::newHttpResponse();
    result->setBody("Not implemented");
    co_return result;
}

drogon::Task<drogon::HttpResponsePtr>
    PackageController::get_packages(drogon::HttpRequestPtr req,
                                    const std::string &branch,
                                    const std::string &repository,
                                    const std::string &architecture) {
    PackageSectionDTO section {branch, repository, architecture};

    const auto packages = co_await m_package_service.get_packages(section);

    Json::Value result;
    for (const auto &package : packages) {
        Json::Value package_json;

        package_json["name"] = package.name;
        package_json["filename"] = package.filepath.string();
        package_json["has_signature"] =
            package.has_signature ? "true" : "false";

        result.append(package_json);
    }

    co_return drogon::HttpResponse::newHttpJsonResponse(result);
}

} // namespace bxt::Presentation
