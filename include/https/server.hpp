#pragma once

#include "base_server.hpp"
#include "https/ssl_context.hpp"
#include "socket.hpp"

#include <net/connection_manager.hpp>
#include <utils/network_stream.hpp>

namespace espresso::https {

class Server : public BaseServer {
private:
    OwnedSocket<PlainSocket> m_socket;

public:
    Server() : Server(Settings{}) {}
    Server(const Settings& settings) : BaseServer(settings), m_socket(AF_INET, SOCK_STREAM, 0) {}

    [[noreturn]] void listen(int port)
    {
        sockaddr_in server_addr{
                .sin_family = AF_INET,
                .sin_port = htons(port),
                .sin_addr = INADDR_ANY,
                .sin_zero = {0},
        };
        m_socket.bind(server_addr);
        m_socket.listen(10);

        https::SSLContext ctx("/home/michele/key/cert.pem", "/home/michele/key/key.pem");
        ConnectionManager<SSLSocket> manager(m_settings.http_workers, m_settings.max_connections, [this](auto& conn) { handle_connection(conn); });

        while (1) {
            sockaddr_in client_addr;
            int client_fd = m_socket.accept(client_addr);
            if (client_fd == -1) {
                throw std::runtime_error("accept() failed");
            }

            manager.push_connection(Connection<SSLSocket>(ctx.create_socket(client_fd), m_settings.recv_timeout));
        }
    }

    void handle_connection(Connection<SSLSocket>& connection)
    {
        auto stream = NetworkStream(connection);
        std::cout << "Trying to receive request..." << std::endl;
        http::Request request = http::Request::receive_from_network(stream);
        std::cout << "Received request" << std::endl;

        http::Response response = m_middleware.run_middlewares(request, [this](http::Request& req) {
            http::Response res;
            m_router.handle(req, res);
            res.headers().insert("Content-Length", std::to_string(res.body().size()));
            return res;
        });
        response.headers().insert("Connection", connection.is_closing() ? "close" : "keep-alive");

        std::string response_str = response.serialize();
        ssize_t written = connection.socket().send(response_str.data(), response_str.size());
        if (written == -1) {
            throw std::runtime_error("write() failed");
        }
    }
};

}// namespace espresso::https