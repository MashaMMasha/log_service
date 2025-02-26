#pragma once
#include <nlohmann/json.hpp>
#include <boost/beast.hpp>

using namespace boost::beast;

class RequestHandler {
public:
    static bool validate_request(const nlohmann::json& request);
    http::response<http::string_body> handle_request(const http::request<http::string_body>& request);
};
