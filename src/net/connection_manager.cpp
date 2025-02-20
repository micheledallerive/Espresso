#include "net/connection_manager.hpp"

#include <http/request.hpp>
#include <utils/network_stream.hpp>

namespace espresso {

ConnectionManager::ConnectionManager(size_t workers, size_t max_idle_connections, HandleFunction&& handlefn) : m_max_idle_connections(max_idle_connections)
{
    m_workers.reserve(workers);
    for (size_t i = 0; i < workers; ++i) {
        m_workers.emplace_back([this](const std::stop_token& stop, const HandleFunction& handle) {
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
            }
        },
                               m_stop_source.get_token(), handlefn);
    }
}
ConnectionManager::~ConnectionManager()
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

Connection ConnectionManager::pop_connection()
{
    std::unique_lock lock(m_mutex);
    m_available.wait(lock, [this] { return !m_pending_connections.empty(); });
    Connection conn = m_pending_connections.front();
    if (m_pending_connections.size() >= m_max_idle_connections)
        conn.set_closing();
    m_pending_connections.pop();
    return conn;
}

void ConnectionManager::push_connection(const Connection& connection)
{
    std::unique_lock lock(m_mutex);
    m_pending_connections.push(connection);
    m_available.notify_one();
}

}// namespace espresso