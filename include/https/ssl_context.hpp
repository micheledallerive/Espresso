#pragma once

#include "https/socket.hpp"

#include <filesystem>

#include <openssl/err.h>
#include <openssl/ssl.h>

namespace espresso::https {

class SSLContext {
private:
    std::unique_ptr<SSL_CTX, decltype(&SSL_CTX_free)> m_ctx{nullptr, SSL_CTX_free};

public:
    SSLContext(const std::filesystem::path& cert_file, const std::filesystem::path& key_file);

    [[nodiscard]] RefSocket<SSLSocket> create_socket(int fd) const
    {
        SSL* ssl = SSL_new(m_ctx.get());
        if (!ssl) {
            throw std::runtime_error("SSL_new() failed");
        }
        return RefSocket<SSLSocket>(fd, ssl);
    }
};

}// namespace espresso::https