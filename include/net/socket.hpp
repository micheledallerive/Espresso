#pragma once

#include <stdexcept>
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

    template<typename Sockaddr>
    Socket& bind(const Sockaddr& addr)
    {
        if (::bind(m_fd, reinterpret_cast<const struct sockaddr*>(&addr), sizeof(Sockaddr)) == -1) {
            perror("bind");
            throw std::runtime_error("bind() failed");
        }
        return *this;
    }

    void listen(int backlog);

    template<typename Sockaddr>
    int accept(Sockaddr& addr)
    {
        socklen_t addr_len = sizeof(Sockaddr);
        return ::accept(m_fd, reinterpret_cast<struct sockaddr*>(&addr), &addr_len);
    }
};

}// namespace espresso