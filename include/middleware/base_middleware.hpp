#pragma once

#include "http/request.hpp"
#include "http/response.hpp"
#include "routing/router.hpp"
#include "middleware/types.hpp"

namespace espresso {

class BaseMiddleware {
public:
    virtual http::Response operator()(http::Request& request, middleware::NextFunctionRef next) = 0;
};

}// namespace espresso
