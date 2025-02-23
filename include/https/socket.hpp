#pragma once

#include "net/socket.hpp"

#include <iostream>

#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/types.h>

namespace espresso::https {

class SSLSocket : public BaseSocket {
private:
    SSL* m_ssl;
    bool m_connected{true};

public:
    SSLSocket(int fd, SSL* ssl);
    ~SSLSocket() = default;

    [[nodiscard]] bool is_connected() const;

    [[nodiscard]] int setup_ssl() const;

    ssize_t recv(void* buf, size_t count, int = 0);

    ssize_t send(const void* buf, size_t count, int = 0) const;

    void close() const;

    [[nodiscard]] bool has_more_data() const;
};
static_assert(SocketConcept<SSLSocket>);

}// namespace espresso::https