#pragma once

#include "orm/reflection/fields.hpp"
#include "orm/reflection/field_wrapper.hpp"

namespace espresso::orm::refl {

template<typename T, typename Callback>
auto view_apply(T &t, Callback &&callback)
{
    const auto values = field_value_ptrs(t);
    [&values, &callback]<size_t... _i>(std::index_sequence<_i...>) {
      (callback(FieldWrapper<T, nth_field<T, _i>()>(std::get<_i>(values))), ...);
    }(std::make_index_sequence<num_fields<T>()>{});
}

}