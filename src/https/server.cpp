#include "https/server.hpp"

namespace espresso::https {
void Server::handle_connection(Connection<SSLSocket>& connection) const
{
    auto stream = NetworkStream(connection);
    http::Request request = http::Request::receive_from_network(stream);

    http::Response response = m_middleware.run_middlewares(request, [this](http::Request& req) {
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
Server::Server(SSLSettings ssl_settings) : Server({}, std::move(ssl_settings)) {}
Server::Server(const Settings& settings, SSLSettings ssl_settings) : BaseServer(settings), m_socket(AF_INET, SOCK_STREAM, 0), m_ssl_settings{std::move(ssl_settings)} {}
void Server::listen(int port)
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

        std::optional<RefSocket<SSLSocket>> client_socket = ctx.create_socket(client_fd);
        if (client_socket.has_value()) {
            manager.push_connection(Connection<SSLSocket>(std::move(client_socket.value()), m_settings.recv_timeout));
        }
    }
}
}// namespace espresso::https