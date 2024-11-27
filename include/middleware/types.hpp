#pragma once

#include "http/request.hpp"
#include "http/response.hpp"
#include <functional>

namespace espresso {

namespace middleware {
using NextFunctionRef = const std::function<http::Response(http::Request&)>&;
using MiddlewareFunction = std::function<http::Response(http::Request&, NextFunctionRef)>;
}// namespace middleware

}// namespace espresso