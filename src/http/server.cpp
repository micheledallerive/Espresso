#include "http/server.hpp"
#include "error/io_exceptions.hpp"
#include "http/request.hpp"
#include "utils/call_detectable.hpp"

#include <arpa/inet.h>
#include <iostream>
#include <net/connection_manager.hpp>
#include <netinet/in.h>
#include <stdexcept>
#include <unistd.h>
#include <utils/network_stream.hpp>
#include <syncstream>

namespace espresso::http {

Server::Server() : Server(Settings{}) {}
Server::Server(const Settings& settings) : m_settings(settings), m_socket(AF_INET, SOCK_STREAM, 0)
{
}

[[noreturn]] void Server::listen(int port)
{
    listen("0.0.0.0", port);
}

[[noreturn]] void Server::listen(std::string_view address, int port)
{
    // convert address to struct in_addr
    struct in_addr addr;
    if (inet_pton(AF_INET, address.data(), &addr) != 1) {
        throw std::runtime_error("inet_pton() failed");
    }

    struct sockaddr_in server_addr {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = addr,
        .sin_zero = {0},
    };

    m_socket
            .bind(server_addr)
            .listen(10);

    ConnectionManager manager(m_settings.http_workers, m_settings.max_connections, [this](auto& conn) { handle_connection(conn); });

    while (1) {
        // accept incoming connection
        struct sockaddr_in client_addr;
        int client_fd = m_socket.accept(client_addr);
        if (client_fd == -1) {
            throw std::runtime_error("accept() failed");
        }

        manager.push_connection(Connection(RefSocket(client_fd), m_settings.recv_timeout));
    }
}
void Server::handle_connection(Connection& connection)
{
    auto stream = NetworkStream(connection);
    Request request = Request::receive_from_network(stream);

    Response response = m_middleware.run_middlewares(request, [this](http::Request& req) {
        http::Response res;
        m_router.handle(req, res);
        res.headers().insert("Content-Length", std::to_string(res.body().size()));
        return res;
    });
    response.headers().insert("Connection", connection.is_closing() ? "close" : "keep-alive");

    std::string response_str = response.serialize();
    ssize_t written = write(connection.socket().fd(), response_str.data(), response_str.size());
    if (written == -1) {
        throw std::runtime_error("write() failed");
    }
}
void Server::middleware(const middleware::MiddlewareFunction& middleware)
{
    m_middleware.add(middleware);
}

}// namespace espresso::http