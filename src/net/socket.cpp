#include "net/socket.hpp"

#include <stdexcept>
#include <unistd.h>

namespace espresso {

BaseSocket::BaseSocket(int fd) : m_fd(fd)
{
    if (m_fd == -1) {
        throw std::runtime_error("socket() failed");
    }
}
Socket::Socket(int domain, int type, int protocol) : BaseSocket(socket(domain, type, protocol))
{
    // set socket option to reuse address
    int optval = 1;
    if (setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
        throw std::runtime_error("setsockopt() failed");
    }
}
Socket::Socket(const Socket& other) : BaseSocket(dup(other.m_fd))
{
}

Socket::Socket(Socket&& other) noexcept : BaseSocket(other.m_fd)
{
    other.m_fd = -1;
}

Socket::~Socket()
{
    if (m_fd != -1) {
        close(m_fd);
    }
}

BaseSocket::operator int() const
{
    return m_fd;
}
void BaseSocket::listen(int backlog)
{
    if (::listen(m_fd, backlog) == -1) {
        throw std::runtime_error("listen() failed");
    }
}

ssize_t BaseSocket::read(void* buf, size_t count)
{
    return ::read(m_fd, buf, count);
}
RefSocket::RefSocket(int fd) : BaseSocket(fd)
{
}
}// namespace espresso
