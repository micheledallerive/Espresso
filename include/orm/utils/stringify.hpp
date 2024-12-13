#pragma once

#include <optional>
#include <string>

namespace espresso::orm {

template<typename T>
struct stringify {
    static std::string to_string(const T& t)
    {
        return std::to_string(t);
    }
};

template<>
struct stringify<std::string> {
    static std::string to_string(const std::string& t)
    {
        return t;
    }
};

template<typename T>
struct stringify<std::optional<T>> {
    static std::string to_string(const std::optional<T>& t)
    {
        if (t.has_value()) {
            return stringify<T>::to_string(t.value());
        }
        return "NULL";
    }
};

template<typename T>
struct quote_stringify {
    static std::string to_string(const T& t)
    {
        return "\"" + stringify<T>::to_string(t) + "\"";
    }
};

template<typename T>
struct quote_stringify<std::optional<T>> {
    static std::string to_string(const std::optional<T>& t)
    {
        if (t.has_value()) {
            return "\"" + stringify<T>::to_string(t.value()) + "\"";
        }
        return "NULL";
    }
};

}// namespace espresso::orm