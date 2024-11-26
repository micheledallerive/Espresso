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
    Path m_path;
    std::map<http::Method, std::vector<Function>> m_handlers;

protected:
    bool handle(http::Request& request, http::Response& response)
    {
        auto matched = m_path.matches(request.path());
        if (!matched) return false;
        if (m_handlers[request.method()].empty()) return false;

        request.set_url_params(std::move(matched.value()));
        for (auto& handler : m_handlers[request.method()]) {
            handler(request, response);
        }
        return true;
    }

public:
    explicit Route(const std::string& path) : m_path(path) {}

    Route& use(http::Method method, const Function& handler)
    {
        m_handlers[method].push_back(handler);
        return *this;
    }

    friend class Router;
};

class Router {
private:
    std::vector<Route> m_routes;
    std::map<std::string, std::size_t> m_routes_ref;

public:
    Router() = default;
    Router& use(const std::string& path, http::Method method, const Route::Function& handler)
    {
        if (m_routes_ref.find(path) != m_routes_ref.end()) {
            m_routes[m_routes_ref[path]].use(method, handler);
        }
        else {
            m_routes.emplace_back(path).use(method, handler);
            m_routes_ref[path] = m_routes.size() - 1;
        }
        return *this;
    }

    Route& route(const std::string& path)
    {
        if (m_routes_ref.find(path) != m_routes_ref.end()) {
            return m_routes[m_routes_ref[path]];
        }
        else {
            m_routes.emplace_back(path);
            m_routes_ref[path] = m_routes.size() - 1;
        }
        return m_routes.back();
    }

    void handle(http::Request& request, http::Response& response)
    {
        for (auto& route : m_routes) {
            if (route.handle(request, response)) return;
        }
    }
};

}// namespace espresso