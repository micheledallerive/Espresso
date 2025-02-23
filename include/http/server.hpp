#pragma once

#include "middleware/middleware_list.hpp"
#include "net/socket.hpp"
#include "routing/router.hpp"

#include "base_server.hpp"
#include <forward_list>
#include <functional>
#include <future>
#include <net/connection.hpp>
#include <string_view>
#include <vector>

namespace espresso::http {

class Server : public BaseServer {
private:
    OwnedSocket<PlainSocket> m_socket;

    void handle_connection(Connection<PlainSocket>& connection);

public:
    Server();
    explicit Server(const Settings& settings);
    ~Server() = default;

    [[noreturn]] void listen(int port);
};

static_assert(ServerConcept<Server>);

}// namespace espresso::http