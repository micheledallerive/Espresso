#pragma once

#include <any>
#include <optional>

namespace espresso::orm {

template<typename T>
struct anycast_if_present {
    static T cast(const std::any& a)
    {
        if (!a.has_value()) {
            return {};
        }
        return std::any_cast<T>(a);
    }
};

template<typename T>
struct anycast_if_present<std::optional<T>> {
    static std::optional<T> cast(const std::any& a)
    {
        if (!a.has_value()) {
            return {};
        }
        return {std::any_cast<T>(a)};
    }
};

}// namespace espresso::orm