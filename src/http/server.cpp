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

    ConnectionManager<PlainSocket> manager(m_settings.http_workers, m_settings.max_connections, [this](auto& conn) { handle_connection(conn); });

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

void Server::handle_connection(Connection<PlainSocket>& connection)
{
    auto stream = NetworkStream(connection);
    Request request = Request::receive_from_network(stream);

    Response response = m_middleware.run_middlewares(request, [this](http::Request& req) {
        http::Response res;
        m_router.handle(req, res);
        res.headers().set("Content-Length", std::to_string(res.body().size()));
        return res;
    });
    response.headers().set("Connection", connection.is_closing() ? "close" : "keep-alive");

    std::string response_str = response.serialize();
    ssize_t written = connection.socket().send(response_str.data(), response_str.size());
    if (written == -1) {
        throw std::runtime_error("write() failed");
    }
}

}// namespace espresso::http