#pragma once

#include "routing/types.hpp"

namespace espresso {

class BaseRouteWithNext {
public:
    BaseRouteWithNext() = default;
    virtual ~BaseRouteWithNext() = default;

    virtual void operator()(const http::Request&, http::Response&, routing::NextFunctionRef) = 0;
};

class BaseRoute {
public:
    BaseRoute() = default;
    virtual ~BaseRoute() = default;

    virtual void operator()(const http::Request&, http::Response&) = 0;
};

}// namespace espresso