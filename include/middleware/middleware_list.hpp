#pragma once

#include "http/request.hpp"
#include "http/response.hpp"
#include "middleware/types.hpp"
#include <functional>
#include <utility>
#include <vector>

namespace espresso {

class MiddlewareList {
private:
    std::vector<middleware::MiddlewareFunction> m_middlewares;

public:
    MiddlewareList() = default;

    http::Response run_middlewares(http::Request&, const std::function<http::Response(http::Request&)>&) const;
    void add(const middleware::MiddlewareFunction& middleware);
};

}// namespace espresso