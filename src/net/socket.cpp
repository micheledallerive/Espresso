#include "net/socket.hpp"

#include <stdexcept>
#include <unistd.h>

namespace espresso {

Socket::Socket(int domain, int type, int protocol) : m_fd(socket(domain, type, protocol))
{
    if (m_fd == -1) {
        throw std::runtime_error("socket() failed");
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
Socket& Socket::bind(const struct sockaddr* addr, socklen_t addrlen)
{
    if (::bind(m_fd, addr, addrlen) == -1) {
        throw std::runtime_error("bind() failed");
    }
    return *this;
}

}// namespace espresso
