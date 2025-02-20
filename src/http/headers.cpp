#include "http/headers.hpp"

namespace espresso::http {
Headers::Headers(Headers::Map headers) : m_headers(std::move(headers))
{
}
void Headers::set(const std::string& name, const std::string& value)
{
    m_headers[name] = value;
}
void Headers::insert(const std::string& name, const std::string& value)
{
    if (m_headers.find(name) == m_headers.end()) {
        m_headers[name] = value;
    }
    else {
        // combine them
        auto& val = m_headers[name];
        val += ",";
        val += value;
    }
}
void Headers::erase(const std::string& name)
{
    m_headers.erase(name);
}
std::string& Headers::operator[](const std::string& name)
{
    return m_headers[name];
}
const std::string& Headers::operator[](const std::string& name) const
{
    return m_headers.at(name);
}
bool Headers::contains(const std::string& name) const
{
    return m_headers.find(name) != m_headers.end();
}
const Headers::Map& Headers::map() const
{
    return m_headers;
}
Headers::Map::iterator Headers::begin()
{
    return m_headers.begin();
}
Headers::Map::iterator Headers::end()
{
    return m_headers.end();
}
Headers::Map::const_iterator Headers::begin() const
{
    return m_headers.begin();
}
Headers::Map::const_iterator Headers::end() const
{
    return m_headers.end();
}
}// namespace espresso::http