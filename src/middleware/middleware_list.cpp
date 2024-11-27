#include "middleware/middleware_list.hpp"

namespace espresso {

http::Response MiddlewareList::run_middlewares(http::Request& request, const std::function<http::Response(http::Request&)>& router)
{
    auto call_chain = [&](this auto&& self, size_t idx, http::Request& req) -> http::Response {
        if (idx < m_middlewares.size())
            return m_middlewares[idx](req, [&](http::Request& r) { return self(idx + 1, r); });
        return router(req);
    };
    return call_chain(0, request);
}
void MiddlewareList::add(const middleware::MiddlewareFunction& middleware) { m_middlewares.push_back(middleware); }
}// namespace espresso