#pragma once

#include "middleware/middleware_list.hpp"
#include "net/socket.hpp"
#include "routing/router.hpp"
#include <functional>
#include <future>
#include <string_view>
#include <vector>

namespace espresso::http {

using namespace std::chrono_literals;
class Server {
public:
    struct Settings {
        static constexpr std::chrono::microseconds DEFAULT_RECV_TIMEOUT{5s};

        std::chrono::microseconds recv_timeout = DEFAULT_RECV_TIMEOUT;
    };

private:
    Settings m_settings;

    Socket m_socket;
    std::vector<std::future<void>> m_workers;

    Router m_router;
    MiddlewareList m_middleware;

    void handle_client(int client_fd);

public:
    Server();
    explicit Server(const Settings &settings);
    Server(const Server&) = delete;
    Server(Server&&) = delete;
    ~Server();

    Router& router() { return m_router; }

    void middleware(const middleware::MiddlewareFunction& middleware);

    [[noreturn]] void listen(int port);
    [[noreturn]] void listen(std::string_view address, int port);
};

}// namespace espresso::http