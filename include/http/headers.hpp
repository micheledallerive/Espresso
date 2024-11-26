#pragma once

#include <map>
#include <string>

namespace espresso::http {

class Headers {
public:
    using Map = std::map<std::string, std::string>;

private:
    Map m_headers;

public:
    Headers() = default;
    Headers(const Map& headers) : m_headers(headers) {}

    void add(const std::string& name, const std::string& value)
    {
        if (m_headers.find(name) == m_headers.end()) {
            m_headers[name.data()] = value.data();
        }
        else {
            // combine them
            auto& val = m_headers[name.data()];
            val += ",";
            val += value.data();
        }
    }

    void remove(const std::string& name)
    {
        m_headers.erase(name);
    }

    [[nodiscard]] const std::string& operator[](const std::string& name) const
    {
        return m_headers.at(name);
    }

    [[nodiscard]] bool contains(const std::string& name) const
    {
        return m_headers.find(name) != m_headers.end();
    }

    [[nodiscard]] const Map& map() const
    {
        return m_headers;
    }

    [[nodiscard]] Map::iterator begin()
    {
        return m_headers.begin();
    }

    [[nodiscard]] Map::iterator end()
    {
        return m_headers.end();
    }

    [[nodiscard]] Map::const_iterator begin() const
    {
        return m_headers.begin();
    }

    [[nodiscard]] Map::const_iterator end() const
    {
        return m_headers.end();
    }
};

}// namespace espresso::http