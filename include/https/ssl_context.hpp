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

    [[nodiscard]] std::optional<RefSocket<SSLSocket>> create_socket(int fd) const;
};

}// namespace espresso::https