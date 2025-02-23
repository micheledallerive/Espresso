#pragma once

#include "utils/string.hpp"
#include <optional>
#include <string>
#include <map>
#include <utility>

namespace espresso::http {

class Headers {
public:
    using Map = std::map<std::string, std::string, case_insensitive_comparator>;

private:
    Map m_headers;

public:
    Headers() = default;
    explicit Headers(Map headers);

    void set(const std::string &name, const std::string &value);
    void insert(const std::string& name, const std::string& value);
    void erase(const std::string& name);

    [[nodiscard]] std::string& operator[](const std::string& name);
    [[nodiscard]] const std::string& operator[](const std::string& name) const;

    [[nodiscard]] bool contains(const std::string& name) const;

    [[nodiscard]] const Map& map() const;

    [[nodiscard]] Map::iterator begin();
    [[nodiscard]] Map::iterator end();

    [[nodiscard]] Map::const_iterator begin() const;
    [[nodiscard]] Map::const_iterator end() const;
};

}// namespace espresso::http