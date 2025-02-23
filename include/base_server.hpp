#pragma once
#include <middleware/middleware_list.hpp>
#include <middleware/types.hpp>
#include <net/connection.hpp>
#include <routing/router.hpp>
#include <utility>

namespace espresso {

using namespace std::chrono_literals;

template<typename T>
concept ServerConcept = requires(T server) {
    { server.listen(std::declval<int>()) } -> std::same_as<void>;
};

class BaseServer {
public:
    struct Settings {
        static constexpr Timeout DEFAULT_RECV_TIMEOUT{5s};
        static constexpr size_t DEFAULT_HTTP_WORKERS{10};
        static constexpr size_t DEFAULT_MAX_CONNECTIONS{1000};

        Timeout recv_timeout = DEFAULT_RECV_TIMEOUT;
        size_t http_workers = DEFAULT_HTTP_WORKERS;
        size_t max_connections = DEFAULT_MAX_CONNECTIONS;
    };

protected:
    Settings m_settings;
    Router m_router;
    MiddlewareList m_middleware;

public:
    BaseServer() : BaseServer(Settings{}) {}
    BaseServer(const BaseServer&) = delete;
    BaseServer(BaseServer&&) = delete;

    explicit BaseServer(const Settings& settings) : m_settings(settings) {}

    ~BaseServer() = default;

    Router& router() { return m_router; }
    void middleware(const middleware::MiddlewareFunction& middleware)
    {
        m_middleware.add(middleware);
    }
};

}// namespace espresso