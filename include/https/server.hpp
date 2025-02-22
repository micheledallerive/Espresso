#pragma once

#include "base_server.hpp"
#include "https/ssl_context.hpp"
#include "socket.hpp"

#include <net/connection_manager.hpp>
#include <utility>
#include <utils/network_stream.hpp>

namespace espresso::https {

class Server : public BaseServer {
public:
    struct SSLSettings {
        std::filesystem::path cert_file;
        std::filesystem::path key_file;
    };

private:
    OwnedSocket<PlainSocket> m_socket;
    SSLSettings m_ssl_settings;

public:
    //Server() : Server({}, {}) {}
    explicit Server(SSLSettings ssl_settings) : Server({}, std::move(ssl_settings)) {}
    Server(const Settings& settings, SSLSettings ssl_settings) : BaseServer(settings), m_ssl_settings{std::move(ssl_settings)}, m_socket(AF_INET, SOCK_STREAM, 0) {}

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

        https::SSLContext ctx(m_ssl_settings.cert_file, m_ssl_settings.key_file);
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
        http::Request request = http::Request::receive_from_network(stream);

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