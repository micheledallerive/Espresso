#pragma once

#include "orm/utils.hpp"
#include <rfl/internal/get_field_names.hpp>
#include <string>

namespace espresso::orm {

template<auto field_ptr>
consteval std::string_view get_field_name_str_view()
{
    using T = typename struct_field_ptr<decltype(field_ptr)>::Struct;
#if defined(__clang__)
    return rfl::internal::get_field_name_str_view<T, rfl::internal::wrap(field_ptr)>();
#else
    return rfl::internal::get_field_name_str_view<T, field_ptr>();
#endif
}

template<auto field_ptr>
std::string get_field_name_str() {
    return std::string(get_field_name_str_view<field_ptr>());
}

}// namespace espresso::orm