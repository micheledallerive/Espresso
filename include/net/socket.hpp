#pragma once

#include <chrono>
#include <cstdio>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

namespace espresso {

template<typename T>
concept SocketConcept = requires(T socket) {
    {
        socket.accept(std::declval<sockaddr_in&>())
    } -> std::same_as<int>;
    {
        socket.bind(std::declval<sockaddr_in&>())
    } -> std::same_as<void>;
    {
        socket.listen(std::declval<int>())
    } -> std::same_as<void>;
    {
        socket.recv(std::declval<void*>(), std::declval<size_t>(), std::declval<int>())
    } -> std::same_as<ssize_t>;
    {
        socket.send(std::declval<const void*>(), std::declval<size_t>(), std::declval<int>())
    } -> std::same_as<ssize_t>;
    {
        socket.close()
    } -> std::same_as<void>;
    {
        socket.is_connected()
    } -> std::same_as<bool>;
    {
        socket.fd()
    } -> std::same_as<int>;
};

class BaseSocket {
protected:
    int m_fd;
    explicit BaseSocket(int fd);

public:
    int accept(sockaddr_in& addr) const;

    void bind(sockaddr_in& addr) const;

    void listen(int backlog) const;

    [[nodiscard]] int fd() const;
};

class PlainSocket : public BaseSocket {
protected:
    explicit PlainSocket(int fd);

public:
    ~PlainSocket() = default;

    ssize_t recv(void* buf, size_t count, int flags = 0) const;

    ssize_t send(const void* buf, size_t count, int flags = 0) const;

    [[nodiscard]] bool is_connected() const;

    void close() const;
};
static_assert(SocketConcept<PlainSocket>);

template<typename Parent>
    requires SocketConcept<Parent>
class RefSocket : public Parent {
public:
    template<typename... Args>
    explicit RefSocket(Args&&... args) : Parent(std::forward<Args>(args)...)
    {
    }
};

template<typename Parent>
    requires SocketConcept<Parent>
class OwnedSocket : public Parent {
public:
    template<typename... Args>
    OwnedSocket(int domain, int type, int protocol, Args&&... args) : Parent(::socket(domain, type, protocol), std::forward<Args>(args)...)
    {
        // set socket option to reuse address
        int optval = 1;
        if (setsockopt(this->fd(), SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
            throw std::runtime_error("setsockopt() failed");
        }
    }

    OwnedSocket(const OwnedSocket<Parent>& other) : Parent(dup(other.fd()))
    {
    }

    OwnedSocket(OwnedSocket<Parent>&& other) noexcept : Parent(other.fd())
    {
        other.m_fd = -1;
    }

    ~OwnedSocket()
    {
        if (this->fd() != -1) {
            this->close();
        }
    }
};

}// namespace espresso