#include "net/socket.hpp"

#include "error/io_exceptions.hpp"
#include <iostream>
#include <stdexcept>
#include <unistd.h>

namespace espresso {

BaseSocket::BaseSocket(int fd) : m_fd(fd), m_timeout()
{
    if (m_fd == -1) {
        throw std::runtime_error("socket() failed");
    }

    FD_ZERO(&m_read_fds);
    FD_SET(m_fd, &m_read_fds);
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
void BaseSocket::set_timeout(std::chrono::microseconds timeout)
{
    auto us_in_s = 1000000;
    m_timeout.tv_sec = timeout.count() / us_in_s;
    m_timeout.tv_usec = timeout.count() % us_in_s;
}
void BaseSocket::listen(int backlog)
{
    if (::listen(m_fd, backlog) == -1) {
        throw std::runtime_error("listen() failed");
    }
}

ssize_t BaseSocket::read(void* buf, size_t count)
{
    int ret = select(m_fd + 1, &m_read_fds, nullptr, nullptr, &m_timeout);
    if (ret == -1) {
        throw std::runtime_error("select() failed");
    }
    else if (ret == 0) {
        throw TimeoutException();
    }
    if (!FD_ISSET(m_fd, &m_read_fds)) {
        return 0;
    }
    return ::read(m_fd, buf, count);
}
RefSocket::RefSocket(int fd) : BaseSocket(fd)
{
}
}// namespace espresso
