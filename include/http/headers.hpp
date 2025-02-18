#pragma once

#include "utils/string.hpp"
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>

namespace espresso::http {

struct case_insensitive_compare {
    bool operator()(const std::string& a, const std::string& b) const
    {
        return equals_case_insensitive(a, b);
    }
};

class Headers {
public:
    using Map = std::unordered_map<std::string, std::string, std::hash<std::string>, case_insensitive_compare>;

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