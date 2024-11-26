#pragma once

#include <stdexcept>
#include <variant>

namespace espresso {

template<typename T>
class Conditional {
private:
    bool m_valid;
    T m_value;

public:
    Conditional() : m_valid(false) {}
    explicit Conditional(const T& value) : m_valid(true), m_value(value) {}
    explicit Conditional(T&& value) : m_valid(true), m_value(std::move(value)) {}

    operator bool() const noexcept { return valid(); }
    [[nodiscard]] bool valid() const noexcept { return m_valid; }
    const T& value() const
    {
        if (!valid()) throw std::runtime_error("No value");
        return m_value;
    }
    T& value() {
        if (!valid()) throw std::runtime_error("No value");
        return m_value;
    }
};

}// namespace espresso