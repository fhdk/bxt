/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2023 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "DeploymentController.h"

namespace bxt::Presentation {
using namespace drogon;

drogon::Task<drogon::HttpResponsePtr>
    DeploymentController::deploy_start(drogon::HttpRequestPtr req) {
    const auto key = req->headers().at("key");

    if (key != m_key) { co_return HttpResponse::newHttpResponse(); }

    const auto result = HttpResponse::newHttpResponse();

    const auto start_ok = co_await m_service.deploy_start();

    if (!start_ok.has_value()) {
        result->setBody(start_ok.error().message());
        co_return result;
    }

    result->setBody(std::to_string(*start_ok));
    co_return result;
}

drogon::Task<drogon::HttpResponsePtr>
    DeploymentController::deploy_push(drogon::HttpRequestPtr req) {
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
        PackageDTO {section, name, app().getUploadPath() + "/" + name,
                    app().getUploadPath() + "/" + signature.getFileName()};

    co_await m_service.deploy_push(dto, session_id);

    auto resp = HttpResponse::newHttpResponse();

    resp->setExpiredTime(0);
    resp->setBody("ok");

    co_return resp;
}

drogon::Task<drogon::HttpResponsePtr>
    DeploymentController::deploy_end(drogon::HttpRequestPtr req) {
    const auto headers = req->getHeaders();
    auto session_id = headers.at("session");
    auto key = headers.at("key");

    co_await m_service.deploy_end(std::stoull(session_id));

    auto resp = HttpResponse::newHttpResponse();

    resp->setBody("ok");

    co_return resp;
}

} // namespace bxt::Presentation
