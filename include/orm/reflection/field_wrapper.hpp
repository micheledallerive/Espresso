#pragma once

#include "orm/utils/types.hpp"
#include "orm/reflection/get_name.hpp"

namespace espresso::orm::refl {

template<typename T, auto ct_field>
class FieldWrapper {
public:
    using Type = clean_type_t<decltype(ct_field)>;

private:
    Type const* m_value_ptr{nullptr};

public:
    constexpr FieldWrapper(Type* value_ptr) : m_value_ptr(value_ptr) {}
    template<typename U>
    constexpr FieldWrapper(U value_ptr) : m_value_ptr(value_ptr) {}

    constexpr std::string name() const
    {
        return std::string(internal::get_field_name<T, ct_field>());
    }

    constexpr const Type* value_ptr() const
    {
        return m_value_ptr;
    }

    constexpr Type* value_ptr()
    {
        return m_value_ptr;
    }
};

}// namespace espresso::orm::refl