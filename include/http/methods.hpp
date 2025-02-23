#pragma once

#include <string>

namespace espresso::http {
enum class Method {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH,
    NONE
};

[[maybe_unused]] static std::string method_to_string(Method method)
{
    switch (method) {
    case Method::GET:
        return "GET";
    case Method::HEAD:
        return "HEAD";
    case Method::POST:
        return "POST";
    case Method::PUT:
        return "PUT";
    case Method::DELETE:
        return "DELETE";
    case Method::CONNECT:
        return "CONNECT";
    case Method::OPTIONS:
        return "OPTIONS";
    case Method::TRACE:
        return "TRACE";
    case Method::PATCH:
        return "PATCH";
    case Method::NONE:
        return "NONE";
    }
    return "NONE";
}

[[maybe_unused]] static Method string_to_method(std::string_view method)
{
    if (method == "GET") {
        return Method::GET;
    }
    else if (method == "HEAD") {
        return Method::HEAD;
    }
    else if (method == "POST") {
        return Method::POST;
    }
    else if (method == "PUT") {
        return Method::PUT;
    }
    else if (method == "DELETE") {
        return Method::DELETE;
    }
    else if (method == "CONNECT") {
        return Method::CONNECT;
    }
    else if (method == "OPTIONS") {
        return Method::OPTIONS;
    }
    else if (method == "TRACE") {
        return Method::TRACE;
    }
    else if (method == "PATCH") {
        return Method::PATCH;
    }
    else {
        return Method::NONE;
    }
}
}// namespace espresso::http