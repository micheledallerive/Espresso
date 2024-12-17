#pragma once

#include "orm/utils/types.hpp"
#include <string>

namespace espresso::orm::refl {

namespace internal {
template<class T>
consteval auto get_type_name()
{
    const auto func_name =
            std::string_view{std::source_location::current().function_name()};
#if defined(__clang__)
    const auto split = func_name.substr(0, func_name.size() - 1);
    return split.substr(split.find("T = ") + 4);
#elif defined(__GNUC__)
    const auto split = func_name.substr(0, func_name.size() - 1);
    return split.substr(split.find("T = ") + 4);
#elif defined(_MSC_VER)
    auto split = func_name.substr(0, func_name.size() - 7);
    split = split.substr(split.find("get_type_name<") + 23);
    auto pos = split.find(" ");
    if (pos != std::string_view::npos) {
        return split.substr(pos + 1);
    }
    return split;
#else
    static_assert(false, "You are using an unsupported compiler.");
#endif
}

template<class T, auto ptr>
consteval auto get_field_name()
{
    const auto func_name =
            std::string_view{std::source_location::current().function_name()};
#if defined(__clang__) || defined(__GNUC__)
    const auto split = func_name.substr(0, func_name.size() - 1);
    return split.substr(split.find_last_of(":.") + 1);
#elif defined(_MSC_VER)
    const auto split = func_name.substr(0, func_name.size() - 7);
    return split.substr(split.rfind("->") + 2);
#else
    static_assert(false, "You are using an unsupported compiler.");
#endif
}
}// namespace internal

template<auto field_ptr>
consteval std::string_view get_field_name()
{
    using T = typename struct_field_ptr<decltype(field_ptr)>::Struct;
    return internal::get_field_name<T, field_ptr>();
}

template<auto field_ptr>
std::string get_field_name_str()
{
    return std::string(get_field_name<field_ptr>());
}

template<typename T>
consteval std::string_view get_type_name()
{
    return internal::get_type_name<T>();
}

template<typename T>
std::string get_type_name_str()
{
    return std::string(get_type_name<T>());
}

}// namespace espresso::orm::refl