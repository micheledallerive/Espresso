#pragma once

#include "socket.hpp"
#include <chrono>
#include <cstring>
#include <error/io_exceptions.hpp>
#include <iostream>
#include <sys/epoll.h>

namespace espresso {

class Connection {
public:
    using Timeout = std::chrono::milliseconds;

private:
    RefSocket m_client;
    int m_epoll_fd;
    Timeout m_timeout;
    epoll_event m_event{};

    bool m_closing{false};

public:
    Connection(const RefSocket& socket,
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

    RefSocket& socket()
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
        close(m_client.fd());
    }

    [[nodiscard]] bool is_active() const
    {
        char buf;
        ssize_t ret = ::recv(m_client.fd(), &buf, 1, MSG_PEEK | MSG_DONTWAIT);
        bool inactive = (ret == 0) || (ret == -1 && errno != EAGAIN);
        return !inactive;
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