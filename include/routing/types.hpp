#pragma once

#include <functional>
#include "http/request.hpp"
#include "http/response.hpp"

namespace espresso::routing {

using NextFunctionRef = const std::function<void()>&;

using RouteFunction = std::function<void(const http::Request&, http::Response&)>;
using RouteFunctionWithNext = std::function<void(const http::Request&, http::Response&, NextFunctionRef)>;

}
