#include "request_handler.h"

bool RequestHandler::validate_request(const nlohmann::json& request) {
    // TODO: add more validation rules
    if (!request.contains("level") || !request.contains("message") || !request.contains("timestamp")) {
        return false;
    }
    return true;
}

http::response<http::string_body> RequestHandler::handle_request(const http::request<http::string_body>& request) {
    http::response<http::string_body> response;
    auto json_request = nlohmann::json::parse(request.body());
    if (!validate_request(json_request)) {
        response.result(http::status::bad_request);
        response.body() = "Invalid request body";
        return response;
    }

    if (request.method() == http::verb::post && request.target() == "/logs") {
        // process request
    } else if (request.method() == http::verb::get && request.target() == "/logs") {
        // process request
    } else {
        response.result(http::status::not_found);
        response.body() = "Page not found";
        return response;
    }
    response.result(http::status::ok);
    response.body() = "Request processed successfully";
    return response;
}