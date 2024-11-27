#include "routing/router.hpp"

namespace espresso {

bool Route::handle(http::Request& request, http::Response& response)
{
    auto matched = m_path.matches(request.path());
    if (!matched) return false;

    request.set_url_params(std::move(matched.value()));
    for (const auto& [method, handler] : m_handlers) {
        if (method != http::Method::NONE && method != request.method()) continue;
        bool call_next = false;
        auto next = [&call_next]() { call_next = true; };
        handler(request, response, next);
        if (!call_next) return true;
    }
    return false;
}
Route::Route(const std::string& path) : m_path(path) {}
Route& Route::use(http::Method method, const Route::Function& handler)
{
    m_handlers.emplace_back(method, handler);
    return *this;
}
Route& Route::use(http::Method method, const Route::NoNextFunction& handler)
{
    m_handlers.emplace_back(method, [handler](const http::Request& request, http::Response& response, Route::NextFunction) {
        handler(request, response);// no next by default(?)
    });
    return *this;
}
void Route::set_handlers(const Route::Handlers& map)
{
    m_handlers = map;
}

//
//
// Router
Router& Router::use(const std::string& path, http::Method method, const Route::Function& handler)
{
    get_route(path).use(method, handler);
    return *this;
}
Router& Router::use(const std::string& path, http::Method method, const Route::NoNextFunction& handler)
{
    get_route(path).use(method, handler);
    return *this;
}
Route& Router::route(const std::string& path)
{
    return get_route(path);
}
void Router::handle(http::Request& request, http::Response& response)
{
    for (auto& route : m_routes) {
        if (route.handle(request, response)) return;
    }
    response.status(404);
}
Route& Router::get_route(const std::string& path)
{
    if (path.empty())
        throw std::invalid_argument("Path cannot be empty");
    auto it = m_routes_ref.find(path);
    if (it == m_routes_ref.end()) {
        m_routes.emplace_back(path);
        m_routes_ref[path] = m_routes.size() - 1;
        return m_routes.back();
    }
    return m_routes[it->second];
}
Router& Router::route(const std::string& base_path, const Router& router)
{
    for (const auto& other_route : router.m_routes) {
        std::string new_path = base_path + other_route.m_path.path();
        auto& route = get_route(new_path);
        route.set_handlers(other_route.m_handlers);
    }
    return *this;
}
}// namespace espresso