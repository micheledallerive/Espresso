#pragma once

#include "middleware/base_middleware.hpp"
#include <nlohmann/json.hpp>

namespace espresso {

class JSONParser : public BaseMiddleware {
public:
    JSONParser() = default;
    ~JSONParser() override = default;

    http::Response operator()(http::Request& request, middleware::NextFunctionRef next) override
    {
        if (request.headers()["Content-Type"] != "application/json") {
            return next(request);
        }

        auto body = request.body();
        if (body.empty()) {
            return next(request);
        }

        try {
            nlohmann::json json = nlohmann::json::parse(body);
            request.set_data("json", json);
            return next(request);
        }
        catch (const nlohmann::json::parse_error& e) {
            http::Response response;
            response.status(400);
            response.write("Invalid JSON");
            return response;
        }
        return {};
    }
};

}// namespace espresso
