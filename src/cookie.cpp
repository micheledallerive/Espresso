#include "cookie.hpp"

namespace espresso {

Cookie::Cookie(std::string_view name, std::string_view value) : m_name(name), m_value(value) {}
Cookie& Cookie::set_path(std::string_view path)
{
    m_path = path;
    return *this;
}
Cookie& Cookie::set_domain(std::string_view domain)
{
    m_domain = domain;
    return *this;
}
Cookie& Cookie::set_max_age(std::size_t age)
{
    max_age = age;
    return *this;
}
Cookie& Cookie::set_secure() noexcept
{
    m_secure = true;
    return *this;
}
Cookie& Cookie::reset_secure() noexcept
{
    m_secure = false;
    return *this;
}
Cookie& Cookie::set_http_only() noexcept
{
    m_http_only = true;
    return *this;
}
Cookie& Cookie::reset_http_only() noexcept
{
    m_http_only = false;
    return *this;
}
std::string Cookie::serialize() const noexcept
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
}