#include "https/ssl_context.hpp"

#include <csignal>
#include <iostream>

namespace espresso::https {

SSLContext::SSLContext(const std::filesystem::path& cert_file, const std::filesystem::path& key_file)
{
    signal(SIGPIPE, SIG_IGN);

    const SSL_METHOD* method = TLS_server_method();
    m_ctx.reset(SSL_CTX_new(method));
    if (!m_ctx) {
        throw std::runtime_error("SSL_CTX_new() failed");
    }

    if (SSL_CTX_use_certificate_file(m_ctx.get(), cert_file.c_str(), SSL_FILETYPE_PEM) <= 0) {
        throw std::runtime_error("Could not set the certificate file. Make sure the file exists and is a valid PEM file");
    }
    if (SSL_CTX_use_PrivateKey_file(m_ctx.get(), key_file.c_str(), SSL_FILETYPE_PEM) <= 0) {
        throw std::runtime_error("Could not set the key file. Make sure the file exists and is a valid PEM file");
    }
}
RefSocket<SSLSocket> SSLContext::create_socket(int fd) const
{
    SSL* ssl = SSL_new(m_ctx.get());
    if (!ssl) {
        throw std::runtime_error("SSL_new() failed");
    }
    return RefSocket<SSLSocket>(fd, ssl);
}
}// namespace espresso::tls