#include "http/headers.hpp"

namespace espresso::http {

void Headers::parse_cookie(std::string_view cookie_str)
{
    auto eq = cookie_str.find('=');
    if (eq != std::string::npos) {
        m_cookies[std::string(cookie_str.substr(0, eq))] = cookie_str.substr(eq + 1);
    }
}
void Headers::parse_cookies(std::string_view cookie)
{
    auto start = cookie.begin();
    auto end = cookie.begin();
    while (end != cookie.end()) {
        if (*end == ';') {
            auto cookie_str = std::string_view(start, end - 1);
            parse_cookie(cookie_str);
            start = end + 1;
        }
        ++end;
    }
    parse_cookie(std::string_view(start, end - 1));
}
Headers::Headers(Headers::Map headers) : m_headers(std::move(headers))
{
    const auto it = m_headers.find("Cookie");
    if (it == m_headers.end()) return;

    parse_cookies(it->second);
}
void Headers::add(const std::string& name, const std::string& value)
{
    if (m_headers.find(name) == m_headers.end()) {
        m_headers[name.data()] = value;
    }
    else {
        // combine them
        auto& val = m_headers[name.data()];
        val += ",";
        val += value;
    }

    if (name == "Cookie") {
        parse_cookies(value);
    }
}
void Headers::remove(const std::string& name)
{
    m_headers.erase(name);

    if (name == "Cookie") {
        m_cookies.clear();
    }
}
const std::string& Headers::operator[](const std::string& name) const
{
    return m_headers.at(name);
}
std::optional<std::string> Headers::get_cookie(const std::string& name) const
{
    auto it = m_cookies.find(name);
    if (it == m_cookies.end()) return {};
    return it->second;
}
bool Headers::contains(const std::string& name) const
{
    return m_headers.find(name) != m_headers.end();
}
const Headers::Map& Headers::map() const
{
    return m_headers;
}
std::map<std::string, std::string>::iterator Headers::begin()
{
    return m_headers.begin();
}
std::map<std::string, std::string>::iterator Headers::end()
{
    return m_headers.end();
}
std::map<std::string, std::string>::const_iterator Headers::begin() const
{
    return m_headers.begin();
}
std::map<std::string, std::string>::const_iterator Headers::end() const
{
    return m_headers.end();
}
}// namespace espresso::http