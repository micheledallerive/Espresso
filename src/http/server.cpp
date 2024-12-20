#include "http/server.hpp"
#include "http/request.hpp"
#include "utils/call_detectable.hpp"

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <unistd.h>

namespace espresso::http {

Server::Server() : m_socket(AF_INET, SOCK_STREAM, 0) {}

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

    while (1) {
        // accept incoming connection
        struct sockaddr_in client_addr;
        int client_fd = m_socket.accept(client_addr);
        if (client_fd == -1) {
            throw std::runtime_error("accept() failed");
        }

        m_workers.push_back(std::async(std::launch::async, [&] {
            try {
                handle_client(client_fd);
            }
            catch (const std::exception& e) {
                std::cerr << "Request crashed: " << e.what() << std::endl;
            }
            close(client_fd);
        }));
    }
}
Server::~Server()
{
    for (auto& worker : m_workers) {
        worker.wait();
    }
}
void Server::handle_client(int client_fd)
{
    char buffer[1024];
    int n = read(client_fd, buffer, sizeof(buffer));
    if (n == -1) {
        throw std::runtime_error("read() failed");
    }

    http::Request request = http::Request::deserialize(std::span(buffer, n));

    http::Response response = m_middleware.run_middlewares(request, [this](http::Request& req) {
        http::Response res;
        m_router.handle(req, res);
        res.headers().insert("Content-Length", std::to_string(res.body().size()));
        return res;
    });

    std::string response_str = response.serialize();
    write(client_fd, response_str.data(), response_str.size());
}
void Server::middleware(const middleware::MiddlewareFunction& middleware)
{
    m_middleware.add(middleware);
}

}// namespace espresso::http