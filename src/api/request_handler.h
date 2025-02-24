#pragma once
#include <nlohmann/json.hpp>

class RequestHandler {
public:
    static bool validate_request(const nlohmann::json& request);
};
