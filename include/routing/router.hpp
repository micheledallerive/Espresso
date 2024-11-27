#pragma once

#include "http/methods.hpp"
#include "http/request.hpp"
#include "http/response.hpp"
#include "path.hpp"
#include <functional>
#include <string>

namespace espresso {

class Router;

class Route {
public:
    using Function = std::function<void(const http::Request&, http::Response&)>;

private:
    using HandlersMap = std::map<http::Method, std::vector<Function>>;
    Path m_path;
    HandlersMap m_handlers;

protected:
    bool handle(http::Request& request, http::Response& response);

    void set_handlers(const HandlersMap &map);
public:
    explicit Route(const std::string& path);

    Route& use(http::Method method, const Function& handler);
    Route& use(http::Method method, std::initializer_list<Function> handlers);

    friend class Router;
};

class Router {
private:
    std::vector<Route> m_routes;
    std::map<std::string, std::size_t> m_routes_ref;

    Route &get_route(const std::string &path);

public:
    Router() = default;
    Router& use(const std::string& path, http::Method method, const Route::Function& handler);

    Route& route(const std::string& path);
    Router& route(const std::string &path, const Router &router);

    void handle(http::Request& request, http::Response& response);
};

}// namespace espresso