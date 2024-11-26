#include "net/socket.hpp"

#include <stdexcept>
#include <unistd.h>

namespace espresso {

Socket::Socket(int domain, int type, int protocol) : m_fd(socket(domain, type, protocol))
{
    if (m_fd == -1) {
        throw std::runtime_error("socket() failed");
    }

    // set socket option to reuse address
    int optval = 1;
    if (setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
        throw std::runtime_error("setsockopt() failed");
    }
}
Socket::Socket(const Socket& other)
{
    m_fd = dup(other.m_fd);
    if (m_fd == -1) {
        throw std::runtime_error("dup() failed");
    }
}

Socket::Socket(Socket&& other) noexcept
{
    m_fd = other.m_fd;
    other.m_fd = -1;
}

Socket::~Socket()
{
    if (m_fd != -1) {
        close(m_fd);
    }
}
Socket::operator int() const
{
    return m_fd;
}
void Socket::listen(int backlog)
{
    if (::listen(m_fd, backlog) == -1) {
        throw std::runtime_error("listen() failed");
    }
}
}// namespace espresso
