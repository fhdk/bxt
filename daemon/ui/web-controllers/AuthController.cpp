/* === This file is part of bxt ===
 *
 *   SPDX-FileCopyrightText: 2022 Artem Grinev <agrinev@manjaro.org>
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *
 */
#include "AuthController.h"

#include <boost/json.hpp>
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/boost-json/traits.h>

drogon::Task<drogon::HttpResponsePtr>
    bxt::UI::AuthController::auth(drogon::HttpRequestPtr req) {
    auto json = *req->getJsonObject();

    const std::string name = json["name"].asString();
    const std::string password = json["password"].asString();

    if (!co_await m_service.auth(name, password)) {
        auto error_resp = drogon::HttpResponse::newHttpResponse();
        error_resp->setStatusCode(drogon::k401Unauthorized);
        co_return error_resp;
    }
    const auto token = jwt::create<jwt::traits::boost_json>()
                           .set_issuer("auth0")
                           .set_type("JWS")
                           .sign(jwt::algorithm::hs256 {"secret"});

    Json::Value result_json;

    result_json["status"] = "ok";
    result_json["token"] = token;

    co_return drogon::HttpResponse::newHttpJsonResponse(result_json);
}

drogon::Task<drogon::HttpResponsePtr>
    bxt::UI::AuthController::verify(drogon::HttpRequestPtr req) {
    //    auto header = req->getHeader("Authorization");
    //    if (header.empty()) {
    //        auto error_resp = drogon::HttpResponse::newHttpResponse();
    //        error_resp->setStatusCode(drogon::k401Unauthorized);
    //        co_return error_resp;
    //    }
    //    auto token = header.substr(7);
    //    auto decoded = jwt::decode<jwt::traits::boost_json>(
    //        token, jwt::algorithm::hs256 {"secret"});
    //    auto verifier = jwt::verify<jwt::traits::boost_json>()
    //                        .allow_algorithm(jwt::algorithm::hs256 {"secret"})
    //                        .with_issuer("auth0");
    //    try {
    //        verifier.verify(decoded);

    //        co_return drogon::HttpResponse::newHttpJsonResponse({{"status",
    //        "ok"}});
    //    } catch (const jwt::token_verification_exception&
    //    verification_exception) {
    //        auto error_resp = drogon::HttpResponse::newHttpResponse();
    //        error_resp->setStatusCode(drogon::k401Unauthorized);
    //        co_return error_resp;
    //    }
}
