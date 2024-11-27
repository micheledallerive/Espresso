#pragma once

#include <functional>

namespace espresso {

class CallDetectable {
public:
    using Function = std::function<void()>;

private:
    bool m_called = false;

public:
    CallDetectable() = default;
    CallDetectable(const CallDetectable&) = delete;
    CallDetectable(CallDetectable&&) = delete;

    Function get_function()
    {
        return [this] {
            this->m_called = true;
        };
    }

    bool was_called() const
    {
        return m_called;
    }

    void reset()
    {
        m_called = false;
    }
};

}// namespace espresso