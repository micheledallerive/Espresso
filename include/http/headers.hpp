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

    void parse_cookie(std::string_view cookie_str);

    void parse_cookies(std::string_view cookie);

public:
    Headers() = default;
    explicit Headers(Map headers);

    void add(const std::string& name, const std::string& value);

    void remove(const std::string& name);

    [[nodiscard]] const std::string& operator[](const std::string& name) const;

    [[nodiscard]] std::optional<std::string_view> get_cookie(const std::string& name) const;

    [[nodiscard]] bool contains(const std::string& name) const;

    [[nodiscard]] const Map& map() const;

    [[nodiscard]] Map::iterator begin();
    [[nodiscard]] Map::iterator end();

    [[nodiscard]] Map::const_iterator begin() const;
    [[nodiscard]] Map::const_iterator end() const;
};

}// namespace espresso::http