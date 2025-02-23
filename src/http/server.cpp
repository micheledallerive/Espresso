#include "http/server.hpp"
#include "error/io_exceptions.hpp"
#include "http/request.hpp"
#include "utils/call_detectable.hpp"

#include <arpa/inet.h>
#include <iostream>
#include <net/connection_manager.hpp>
#include <netinet/in.h>
#include <stdexcept>
#include <syncstream>
#include <unistd.h>
#include <utils/network_stream.hpp>

namespace espresso::http {

Server::Server() : Server(Settings{}) {}
Server::Server(const Settings& settings) : BaseServer(settings), m_socket(AF_INET, SOCK_STREAM, 0)
{
}

[[noreturn]] void Server::listen(int port)
{
    struct sockaddr_in server_addr {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = INADDR_ANY,
        .sin_zero = {0},
    };

    m_socket.bind(server_addr);
    m_socket.listen(10);

    ConnectionManager<PlainSocket> manager(m_settings.http_workers, m_settings.max_connections, [this](Connection<PlainSocket>& conn) { handle_connection(conn); });

    while (1) {
        // accept incoming connection
        struct sockaddr_in client_addr;
        int client_fd = m_socket.accept(client_addr);
        if (client_fd == -1) {
            throw std::runtime_error("accept() failed");
        }

        manager.push_connection(Connection(RefSocket<PlainSocket>(client_fd), m_settings.recv_timeout));
    }
}

}// namespace espresso::http