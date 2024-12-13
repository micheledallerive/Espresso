#pragma once

#include <utility>

namespace espresso {

template<typename T>
class MaybeUninit {
private:
    alignas(T) unsigned char m_data[sizeof(T)]{};
    bool m_initialized = false;

public:
    MaybeUninit() = default;

    ~MaybeUninit()
    {
        if (m_initialized) {
            reinterpret_cast<T*>(m_data)->~T();
        }
    }

    template<typename... Args>
    void emplace(Args&&... args)
    {
        if (m_initialized) {
            reinterpret_cast<T*>(m_data)->~T();
        }
        new (m_data) T(std::forward<Args>(args)...);
        m_initialized = true;
    }

    void emplace(const T& value)
    {
        if (m_initialized) {
            reinterpret_cast<T*>(m_data)->~T();
        }
        new (m_data) T(value);
        m_initialized = true;
    }

    T& get()
    {
        return *reinterpret_cast<T*>(m_data);
    }
};

}// namespace espresso