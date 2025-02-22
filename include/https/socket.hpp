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

public:
    SSLSocket(int fd, SSL* ssl) : BaseSocket(fd), m_ssl(ssl)
    {
        SSL_set_fd(m_ssl, m_fd);
        if (SSL_accept(m_ssl) <= 0) {
            ERR_print_errors_fp(stderr);
            throw std::runtime_error("SSL_accept() failed");
        }
    }
    ~SSLSocket() = default;

    [[nodiscard]] bool is_connected() const
    {
        return SSL_is_init_finished(m_ssl) && SSL_get_shutdown(m_ssl) == 0;
    }

    ssize_t recv(void* buf, size_t count, int = 0) const
    {
        size_t amount = 0;
        int ret = SSL_read_ex(m_ssl, buf, count, &amount);
        // if the return value is 0 or -1, check the error with SSL_get_error()
        // if the client disconnected, return 0
        if (ret == 1) {
            return amount;
        }
        int err = SSL_get_error(m_ssl, ret);
        // print the error

        if (err == SSL_ERROR_ZERO_RETURN) {
            return 0;
        }
        return -1;
    }

    ssize_t send(const void* buf, size_t count, int = 0) const
    {
        size_t amount;
        int ret = SSL_write_ex(m_ssl, buf, count, &amount);
        if (ret == 1) {
            return amount;
        }
        return -1;
    }

    void close() const
    {
        SSL_shutdown(m_ssl);
        SSL_free(m_ssl);
        ::close(m_fd);
    }
};
static_assert(SocketConcept<SSLSocket>);

}// namespace espresso::https