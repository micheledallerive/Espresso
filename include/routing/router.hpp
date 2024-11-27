#pragma once

#include "http/methods.hpp"
#include "http/request.hpp"
#include "http/response.hpp"
#include "path.hpp"
#include <functional>
#include <string>

namespace espresso {

class Router;

#define ROUTE_DECLARE_METHOD(method, enum_value) \
    Route& method(const Function& handler)       \
    {                                            \
        return use(enum_value, handler);         \
    }

#define ROUTER_DECLARE_METHOD(method, enum_value)                           \
    Router& method(const std::string& path, const Route::Function& handler) \
    {                                                                       \
        return use(path, enum_value, handler);                              \
    }

class Route {
public:
    using NextFunction = const std::function<void()>&;
    using Function = std::function<void(const http::Request&, http::Response&, NextFunction)>;

private:
    using Handlers = std::vector<std::pair<http::Method, Function>>;
    Path m_path;
    Handlers m_handlers;

protected:
    bool handle(http::Request& request, http::Response& response);

    void set_handlers(const Handlers& map);

public:
    explicit Route(const std::string& path);

    Route& use(http::Method method, const Function& handler);
    ROUTE_DECLARE_METHOD(get, http::Method::GET);
    ROUTE_DECLARE_METHOD(head, http::Method::HEAD);
    ROUTE_DECLARE_METHOD(post, http::Method::POST);
    ROUTE_DECLARE_METHOD(put, http::Method::PUT);
    ROUTE_DECLARE_METHOD(Delete, http::Method::DELETE);
    ROUTE_DECLARE_METHOD(connect, http::Method::CONNECT);
    ROUTE_DECLARE_METHOD(options, http::Method::OPTIONS);
    ROUTE_DECLARE_METHOD(trace, http::Method::TRACE);
    ROUTE_DECLARE_METHOD(patch, http::Method::PATCH);

    friend class Router;
};

class Router {
private:
    std::vector<Route> m_routes;
    std::map<std::string, std::size_t> m_routes_ref;

    Route& get_route(const std::string& path);

public:
    Router() = default;
    Router& use(const std::string& path, http::Method method, const Route::Function& handler);
    ROUTER_DECLARE_METHOD(get, http::Method::GET);
    ROUTER_DECLARE_METHOD(head, http::Method::HEAD);
    ROUTER_DECLARE_METHOD(post, http::Method::POST);
    ROUTER_DECLARE_METHOD(put, http::Method::PUT);
    ROUTER_DECLARE_METHOD(Delete, http::Method::DELETE);
    ROUTER_DECLARE_METHOD(connect, http::Method::CONNECT);
    ROUTER_DECLARE_METHOD(options, http::Method::OPTIONS);
    ROUTER_DECLARE_METHOD(trace, http::Method::TRACE);
    ROUTER_DECLARE_METHOD(patch, http::Method::PATCH);

    Route& route(const std::string& path);
    Router& route(const std::string& path, const Router& router);

    void handle(http::Request& request, http::Response& response);
};

}// namespace espresso