#pragma once

#include <cstdio>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

namespace espresso {

class BaseSocket {
protected:
    int m_fd;

    explicit BaseSocket(int fd);
    virtual ~BaseSocket() = default;

public:
    explicit operator int() const;
    [[nodiscard]] int fd() const
    {
        return m_fd;
    }

    template<typename Sockaddr>
    BaseSocket& bind(const Sockaddr& addr)
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

    ssize_t read(void* buf, size_t count);
};

/**
 * Socket that wraps a file descriptor with no life-time management.
 *
 * RefSocket is used whenever the interface of the BaseSocket class needs to be used, but the socket is not owned by the class.
 * The user must ensure that the file descriptor stays available for the entire lifetime of the RefSocket object.
 */
class RefSocket : public BaseSocket {
public:
    explicit RefSocket(int fd);
    ~RefSocket() override = default;
};

/**
 * Socket that owns a socket file descriptor.
 * On duplication, the file descriptor is duplicated; on destruction, the file descriptor is closed.
 */
class Socket : public BaseSocket {
public:
    Socket(int domain, int type, int protocol);
    Socket(const Socket& other);
    Socket(Socket&& other) noexcept;
    ~Socket() override;
};

}// namespace espresso