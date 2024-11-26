#pragma once

#include "net/socket.hpp"
#include "routing/router.hpp"
#include <future>
#include <string_view>
#include <vector>

namespace espresso::http {

class Server {
private:
    Socket m_socket;
    std::vector<std::future<void>> m_workers;
    Router m_router;

    void handle_client(int client_fd);

public:
    Server();
    Server(const Server &) = delete;
    Server(Server &&) = delete;
    ~Server();

    Router &router() { return m_router; }

    [[noreturn]] void listen(int port);
    [[noreturn]] void listen(std::string_view address, int port);
};

}// namespace espresso::http