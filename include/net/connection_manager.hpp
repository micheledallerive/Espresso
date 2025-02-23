#pragma once
#include "net/connection.hpp"

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace espresso {

template<typename SocketType>
class ConnectionManager {
public:
    static constexpr size_t DEFAULT_WORKERS = 10;
    static constexpr size_t DEFAULT_IDLE_CONNECTIONS = 100;

    using HandleFunction = std::function<void(Connection<SocketType>&)>;

private:
    std::queue<Connection<SocketType>> m_pending_connections;
    std::mutex m_mutex;
    std::condition_variable m_available;

    size_t m_max_idle_connections;

    std::vector<std::jthread> m_workers;
    std::stop_source m_stop_source{};

public:
    explicit ConnectionManager(size_t workers, size_t max_idle_connections, HandleFunction&& handle) : m_max_idle_connections(max_idle_connections)
    {
        m_workers.reserve(workers);
        for (size_t i = 0; i < workers; ++i) {
            m_workers.emplace_back([this, handle](const std::stop_token& stop) {
                while (!stop.stop_requested()) {
                    Connection conn = pop_connection();
                    conn.wait_for_data();
                    if (!conn.is_active()) {
                        conn.kill();
                        continue;
                    }
                    try {
                        handle(conn);
                        if (conn.is_closing()) {
                            conn.kill();
                        }
                        else
                            push_connection(conn);
                    }
                    catch (std::runtime_error& e) {
                        std::cerr << "Exception: " << e.what() << std::endl;
                        conn.kill();
                    }
                    catch (...) {
                        std::cerr << "Generic exception" << std::endl;
                        conn.kill();
                    }
                }
            },
                                   m_stop_source.get_token());
        }
    }
    ~ConnectionManager()
    {
        m_stop_source.request_stop();
        for (auto& worker : m_workers) {
            worker.join();
        }

        while (!m_pending_connections.empty()) {
            m_pending_connections.front().kill();
            m_pending_connections.pop();
        }
    }

    void push_connection(const Connection<SocketType>& connection)
    {
        std::unique_lock lock(m_mutex);
        m_pending_connections.push(connection);
        m_available.notify_one();
    }

private:
    Connection<SocketType> pop_connection()
    {
        std::unique_lock lock(m_mutex);
        m_available.wait(lock, [this] { return !m_pending_connections.empty(); });
        Connection conn = std::move(m_pending_connections.front());
        if (m_pending_connections.size() >= m_max_idle_connections)
            conn.set_closing();
        m_pending_connections.pop();
        return conn;
    }
};

}// namespace espresso