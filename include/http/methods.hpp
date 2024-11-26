#pragma once

#include <string_view>

namespace espresso::http {
enum Method {
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

[[maybe_unused]] static std::string_view method_to_string(Method method)
{
    switch (method) {
    case GET:
        return "GET";
    case HEAD:
        return "HEAD";
    case POST:
        return "POST";
    case PUT:
        return "PUT";
    case DELETE:
        return "DELETE";
    case CONNECT:
        return "CONNECT";
    case OPTIONS:
        return "OPTIONS";
    case TRACE:
        return "TRACE";
    case PATCH:
        return "PATCH";
    case NONE:
        return "NONE";
    }
    return "NONE";
}

static Method string_to_method(std::string_view method)
{
    if (method == "GET") {
        return GET;
    }
    else if (method == "HEAD") {
        return HEAD;
    }
    else if (method == "POST") {
        return POST;
    }
    else if (method == "PUT") {
        return PUT;
    }
    else if (method == "DELETE") {
        return DELETE;
    }
    else if (method == "CONNECT") {
        return CONNECT;
    }
    else if (method == "OPTIONS") {
        return OPTIONS;
    }
    else if (method == "TRACE") {
        return TRACE;
    }
    else if (method == "PATCH") {
        return PATCH;
    }
    else {
        return NONE;
    }
}
}// namespace espresso::http