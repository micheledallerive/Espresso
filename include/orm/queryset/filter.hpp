#pragma once

#include <string>
#include <string_view>

namespace espresso::orm {

class Filter {
private:
    std::string m_string;

public:
    Filter(std::string left, std::string op, std::string right)
        : m_string(std::move(left) + " " + std::move(op) + " " + std::move(right))
    {}

    [[nodiscard]] const std::string& to_string() const
    {
        return m_string;
    }
};

}// namespace espresso::orm