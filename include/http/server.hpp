#pragma once

#include "middleware/middleware_list.hpp"
#include "net/socket.hpp"
#include "routing/router.hpp"

#include <forward_list>
#include <functional>
#include <future>
#include <net/connection.hpp>
#include <string_view>
#include <vector>

namespace espresso::http {

using namespace std::chrono_literals;
class Server {
public:
    struct Settings {
        static constexpr Connection::Timeout DEFAULT_RECV_TIMEOUT{5s};

        Connection::Timeout recv_timeout = DEFAULT_RECV_TIMEOUT;
    };

private:
    Settings m_settings;

    Socket m_socket;

    std::forward_list<Connection> m_pending_connections;

    Router m_router;
    MiddlewareList m_middleware;

    void handle_connection(Connection &connection);

public:
    Server();
    explicit Server(const Settings& settings);
    Server(const Server&) = delete;
    Server(Server&&) = delete;
    ~Server() = default;

    Router& router() { return m_router; }

    void middleware(const middleware::MiddlewareFunction& middleware);

    [[noreturn]] void listen(int port);
    [[noreturn]] void listen(std::string_view address, int port);
};

}// namespace espresso::http