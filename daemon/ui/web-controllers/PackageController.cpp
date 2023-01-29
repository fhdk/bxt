/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "PackageController.h"

#include <drogon/MultiPart.h>

namespace bxt::UI {
using namespace drogon;

drogon::Task<HttpResponsePtr> PackageController::deploy(HttpRequestPtr req) {
    MultiPartParser file_upload;

    int ok = file_upload.parse(req);
    if (ok < 0) { co_return HttpResponse::newHttpResponse(); }

    const auto headers = req->getHeaders();

    auto token = headers.at("token");

    auto files_map = file_upload.getFilesMap();
    auto file = files_map.at("file");

    auto params_map = file_upload.getParameters();

    auto branch = params_map["branch"];
    auto repo = params_map["repo"];
    auto arch = params_map["architecture"];

    auto section = PackageSectionDTO {
        .branch = branch, .repository = repo, .architecture = arch};

    file.save();

    auto name = file.getFileName();

    auto dto = PackageDTO {section, name, app().getUploadPath() + "/" + name};

    co_await m_service.deploy(dto);

    Json::Value ret;

    ret["result"] = "ok";
    ret["name"] = file.getFileName();
    ret["section"] = std::string(section);

    auto resp = HttpResponse::newHttpJsonResponse(ret);

    resp->setExpiredTime(0);

    co_return resp;
}

} // namespace bxt::UI
