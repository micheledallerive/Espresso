#pragma once

#include <sys/socket.h>

namespace espresso {

class Socket {
protected:
    int m_fd = -1;

public:
    Socket(int domain, int type, int protocol);
    Socket(const Socket& other);
    Socket(Socket&& other) noexcept;
    ~Socket();

    explicit operator int() const;

    Socket& bind(const struct sockaddr* addr, socklen_t addrlen);
};

}// namespace espresso