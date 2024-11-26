#pragma once

#include <map>
#include <optional>
#include <string>
#include <utility>

namespace espresso::http {

class Headers {
public:
    using Map = std::map<std::string, std::string>;

private:
    Map m_headers;
    Map m_cookies;

    void parse_cookie(std::string_view cookie_str) {
        auto eq = cookie_str.find('=');
        if (eq != std::string::npos) {
            m_cookies[std::string(cookie_str.substr(0, eq))] = cookie_str.substr(eq + 1);
        }
    }

    void parse_cookies(std::string_view cookie)
    {
        auto start = cookie.begin();
        auto end = cookie.begin();
        while (end != cookie.end()) {
            if (*end == ';') {
                auto cookie_str = std::string_view(start, end-1);
                parse_cookie(cookie_str);
                start = end + 1;
            }
            ++end;
        }
        parse_cookie(std::string_view(start, end-1));
    }

public:
    Headers() = default;
    explicit Headers(Map headers) : m_headers(std::move(headers))
    {
        const auto it = m_headers.find("Cookie");
        if (it == m_headers.end()) return;

        parse_cookies(it->second);
    }

    void add(const std::string& name, const std::string& value)
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

    void remove(const std::string& name)
    {
        m_headers.erase(name);

        if (name == "Cookie") {
            m_cookies.clear();
        }
    }

    [[nodiscard]] const std::string& operator[](const std::string& name) const
    {
        return m_headers.at(name);
    }

    [[nodiscard]] std::optional<std::string_view> get_cookie(const std::string & name) const
    {
        auto it = m_cookies.find(name);
        if (it == m_cookies.end()) return {};
        return it->second;
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