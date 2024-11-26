#pragma once

#include <sstream>
#include <string>
#include <string_view>

namespace espresso {

class Cookie {
private:
    std::string m_name;
    std::string m_value;

    std::string m_path{};
    std::string m_domain{};
    std::size_t max_age{0};

    bool m_secure{false};
    bool m_http_only{false};

public:
    Cookie(std::string_view name, std::string_view value) : m_name(name), m_value(value) {}

    Cookie& set_path(std::string_view path)
    {
        m_path = path;
        return *this;
    }

    Cookie& set_domain(std::string_view domain)
    {
        m_domain = domain;
        return *this;
    }

    Cookie& set_max_age(std::size_t age)
    {
        max_age = age;
        return *this;
    }

    Cookie& set_secure() noexcept
    {
        m_secure = true;
        return *this;
    }
    Cookie& reset_secure() noexcept
    {
        m_secure = false;
        return *this;
    }

    Cookie& set_http_only() noexcept
    {
        m_http_only = true;
        return *this;
    }
    Cookie& reset_http_only() noexcept
    {
        m_http_only = false;
        return *this;
    }

    std::string serialize() const noexcept
    {
        std::stringstream ss;
        ss << m_name << "=" << m_value;
        if (!m_path.empty()) {
            ss << "; Path=" << m_path;
        }
        if (!m_domain.empty()) {
            ss << "; Domain=" << m_domain;
        }
        if (max_age != 0) {
            ss << "; Max-Age=" << max_age;
        }
        if (m_secure) {
            ss << "; Secure";
        }
        if (m_http_only) {
            ss << "; HttpOnly";
        }
        return ss.str();
    }
};

}// namespace espresso