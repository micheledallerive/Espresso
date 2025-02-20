#pragma once
#include "net/connection.hpp"

#include "ds/tsqueue.hpp"
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
    tsqueue<Connection> m_waiting_connections;
    size_t m_max_idle_connections;

    std::vector<std::jthread> m_workers;
    std::stop_source m_stop_source{};

    Connection pop_connection();
    void push_connection(Connection &&connection);
public:
    explicit ConnectionManager(size_t workers, size_t max_idle_connections, HandleFunction &&handle);
    ~ConnectionManager();

    void add_connection(const Connection& connection);

};

}// namespace espresso