#include "net/connection_manager.hpp"

#include <http/request.hpp>
#include <utils/network_stream.hpp>

namespace espresso {

ConnectionManager::ConnectionManager(size_t workers, size_t max_idle_connections, HandleFunction&& handle) : m_max_idle_connections(max_idle_connections)
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
                    push_connection(std::move(conn));
                }
                catch (std::runtime_error& e) {
                    std::cerr << "Exception: " << e.what() << std::endl;
                    conn.kill();
                }
                catch (...) {
                    std::cerr << "Generic exception, molto male" << std::endl;
                    conn.kill();
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        },
                               m_stop_source.get_token());
    }
}
ConnectionManager::~ConnectionManager()
{
    m_stop_source.request_stop();
    for (auto& worker : m_workers) {
        worker.join();
    }

    std::queue<Connection>& connections_queue = m_waiting_connections.queue();
    while (!connections_queue.empty()) {
        connections_queue.front().kill();
        connections_queue.pop();
    }
}

void ConnectionManager::add_connection(const Connection& connection)
{
    m_waiting_connections.produce(connection);
}
Connection ConnectionManager::pop_connection()
{
    return m_waiting_connections.consume();
}

void ConnectionManager::push_connection(Connection&& connection)
{
    std::optional<Connection> oldest = m_waiting_connections.consume_if([this](const std::queue<Connection>& queue) {
        return queue.size() >= m_max_idle_connections;
    });
    if (oldest.has_value()) {
        std::cout << "Evicting existing request because the max was reached" << std::endl;
        oldest->kill();
    }
    m_waiting_connections.produce(std::forward<Connection>(connection));
}

}// namespace espresso