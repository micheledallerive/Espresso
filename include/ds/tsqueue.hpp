#pragma once
#include <cassert>
#include <condition_variable>
#include <functional>
#include <queue>

namespace espresso {

template<typename T>
class tsqueue {
private:
    std::queue<T> m_queue;
    std::condition_variable m_non_empty;
    std::mutex m_mutex;

public:
    explicit tsqueue() = default;

    void produce(const T& item)
    {
        std::unique_lock lock(m_mutex);
        m_queue.push(item);
        m_non_empty.notify_one();
    }

    T consume()
    {
        std::unique_lock lock(m_mutex);
        m_non_empty.wait(lock, [this] { return !m_queue.empty(); });
        T item = std::move(m_queue.front());
        m_queue.pop();
        std::cout << "Size after consume " << m_queue.size() << std::endl;
        return std::move(item);
    }

    std::optional<T> consume_if(std::function<bool(const std::queue<T>&)>&& predicate)
    {
        std::unique_lock lock(m_mutex);
        if (!predicate(m_queue)) {
            return std::nullopt;
        }
        assert(!m_queue.empty());
        //m_non_empty.wait(lock, [this] { return !m_queue.empty(); });
        T item = std::move(m_queue.front());
        m_queue.pop();
        return std::move(item);
    }

    std::queue<T>& queue()
    {
        return m_queue;
    }

    std::mutex& mutex()
    {
        return m_mutex;
    }
};

}// namespace espresso