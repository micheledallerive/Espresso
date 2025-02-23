#include "net/socket.hpp"

namespace espresso {

BaseSocket::BaseSocket(int fd) : m_fd(fd) {}
int BaseSocket::accept(sockaddr_in& addr) const
{
    socklen_t addr_len = sizeof(struct sockaddr_in);
    return ::accept(m_fd, reinterpret_cast<struct sockaddr*>(&addr), &addr_len);
}
void BaseSocket::bind(sockaddr_in& addr) const
{
    if (::bind(m_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(struct sockaddr_in)) == -1) {
        throw std::runtime_error("bind() failed");
    }
}
void BaseSocket::listen(int backlog) const
{
    if (::listen(m_fd, backlog) == -1) {
        throw std::runtime_error("listen() failed");
    }
}
int BaseSocket::fd() const
{
    return m_fd;
}
PlainSocket::PlainSocket(int fd) : BaseSocket(fd) {}
ssize_t PlainSocket::recv(void* buf, size_t count, int flags) const
{
    return ::recv(m_fd, buf, count, flags);
}
ssize_t PlainSocket::send(const void* buf, size_t count, int flags) const
{
    return ::send(m_fd, buf, count, flags);
}
bool PlainSocket::is_connected() const
{
    char buf;
    ssize_t ret = recv(&buf, 1, MSG_PEEK | MSG_DONTWAIT);
    bool inactive = (ret == 0) || (ret == -1 && errno != EAGAIN);
    return !inactive;
}
void PlainSocket::close() const
{
    ::close(m_fd);
}

}// namespace espresso
