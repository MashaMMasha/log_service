#include "request_handler.h"

bool RequestHandler::validate_request(const nlohmann::json& request) {
    // TODO: add more validation rules
    if (!request.contains("level") || !request.contains("message")) {
        return false;
    }
    return true;
}