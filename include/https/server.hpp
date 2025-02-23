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
    explicit Server(SSLSettings ssl_settings);
    Server(const Settings& settings, SSLSettings ssl_settings);

    [[noreturn]] void listen(int port);
};

}// namespace espresso::https