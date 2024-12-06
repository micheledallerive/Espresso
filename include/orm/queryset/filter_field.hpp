#pragma once

#include "orm/queryset/filter.hpp"
#include "orm/reflection/field_name.hpp"
#include "orm/utils.hpp"
#include "rfl/internal/get_field_names.hpp"

namespace espresso::orm {

template<typename T>
struct stringify {
    static std::string to_string(T&& value)
    {
        return std::to_string(std::forward<T>(value));
    }
};

template<typename T>
    requires std::convertible_to<T, std::string_view>
struct stringify<T> {
    static std::string to_string(T&& value)
    {
        return std::forward<T>(value);
    }
};

template<typename T>
    requires std::is_member_pointer_v<T>
struct stringify<T> {
    static std::string to_string(T&& value)
    {
        return get_field_name_str<T>();
    }
};

template<auto field_ptr>
class FilterField {
public:
    using Model = struct_field_ptr<decltype(field_ptr)>::Struct;
    using FieldType = struct_field_ptr<decltype(field_ptr)>::Field;
    FilterField() = default;
    ~FilterField() = default;

    template<typename T>
    auto operator==(T&& value) const
    {
        return Filter{get_field_name_str<field_ptr>(), "==", stringify<T>::to_string(std::forward<T>(value))};
    }

    template<typename T>
    auto operator!=(T&& value) const
    {
        return Filter{get_field_name_str<field_ptr>(), "!=", stringify<T>::to_string(std::forward<T>(value))};
    }

    template<typename T>
    auto operator<(T&& value) const
    {
        return Filter{get_field_name_str<field_ptr>(), "<", stringify<T>::to_string(std::forward<T>(value))};
    }

    template<typename T>
    auto operator<=(T&& value) const
    {
        return Filter{get_field_name_str<field_ptr>(), "<=", stringify<T>::to_string(std::forward<T>(value))};
    }

    template<typename T>
    auto operator>(T&& value) const
    {
        return Filter{get_field_name_str<field_ptr>(), ">", stringify<T>::to_string(std::forward<T>(value))};
    }

    template<typename T>
    auto operator>=(T&& value) const
    {
        return Filter{get_field_name_str<field_ptr>(), ">=", stringify<T>::to_string(std::forward<T>(value))};
    }
};

template<auto field_ptr>
static constexpr FilterField<field_ptr> field{};
}// namespace espresso::orm