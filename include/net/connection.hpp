#pragma once

#include "socket.hpp"
#include <chrono>
#include <cstring>
#include <error/io_exceptions.hpp>
#include <iostream>
#include <sys/epoll.h>

namespace espresso {

using Timeout = std::chrono::milliseconds;

template<SocketConcept SocketType>
class Connection {
private:
    RefSocket<SocketType> m_client;
    int m_epoll_fd;
    Timeout m_timeout;
    epoll_event m_event{};

    bool m_closing{false};

public:
    Connection(const RefSocket<SocketType>& socket,
               const Timeout& timeout) : m_client(socket),
                                         m_epoll_fd(epoll_create1(0)),
                                         m_timeout(timeout)
    {
        if (timeout.count() > std::numeric_limits<int>::max()) {
            throw std::runtime_error("timeout value is too large, maximum number is " + std::to_string(std::numeric_limits<int>::max()));
        }

        m_event.events = EPOLLIN;
        m_event.data.fd = socket.fd();

        int epoll_ctl_ret = epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, socket.fd(), &m_event);
        if (epoll_ctl_ret == -1) {
            throw std::runtime_error("epoll_ctl() failed");
        }
    }
    //Connection(const Connection&) = delete;

    RefSocket<SocketType>& socket()
    {
        return m_client;
    }

    void wait_for_data()
    {
        struct epoll_event event;
        int nfds = epoll_wait(m_epoll_fd, &event, 1, m_timeout.count());
        if (nfds == -1) {
            perror("epoll_wait()");
            throw std::runtime_error("epoll_wait() failed");
        }
        else if (nfds == 0) {
            throw TimeoutException();
        }
    }

    void kill()
    {
        close(m_epoll_fd);
        m_client.close();
    }

    [[nodiscard]] bool is_active() const
    {
        return m_client.is_connected();
    }

    void set_closing()
    {
        m_closing = true;
    }

    [[nodiscard]] bool is_closing() const
    {
        return m_closing;
    }
};

}// namespace espresso