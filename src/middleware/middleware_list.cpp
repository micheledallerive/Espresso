#include "middleware/middleware_list.hpp"

namespace espresso {

http::Response MiddlewareList::run_middlewares(http::Request& request, const std::function<http::Response(http::Request&)>& router)
{
    size_t next_idx = 0;
    auto call_chain = [&](auto&& self, size_t idx, http::Request& req) -> http::Response {
        if (idx != next_idx) {
            throw std::runtime_error("Each middleware must be called exactly once");
        }
        ++next_idx;
        if (idx < m_middlewares.size())
            return m_middlewares[idx](req, [&](http::Request& r) { return self(self, idx + 1, r); });
        return router(req);
    };
    return call_chain(call_chain, 0, request);
}
void MiddlewareList::add(const middleware::MiddlewareFunction& middleware) { m_middlewares.push_back(middleware); }
}// namespace espresso