#pragma once
#include "net/connection.hpp"

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace espresso {

class ConnectionManager {
public:
    static constexpr size_t DEFAULT_WORKERS = 10;
    static constexpr size_t DEFAULT_IDLE_CONNECTIONS = 100;

    using HandleFunction = std::function<void(Connection&)>;

private:
    std::queue<Connection> m_pending_connections;
    std::mutex m_mutex;
    std::condition_variable m_available;

    size_t m_max_idle_connections;

    std::vector<std::jthread> m_workers;
    std::stop_source m_stop_source{};

    Connection pop_connection();
public:
    explicit ConnectionManager(size_t workers, size_t max_idle_connections, HandleFunction &&handle);
    ~ConnectionManager();

    void push_connection(const Connection &connection);

};

}// namespace espresso