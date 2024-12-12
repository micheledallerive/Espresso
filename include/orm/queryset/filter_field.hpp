#pragma once

#include "orm/concepts.hpp"
#include "orm/queryset/filter.hpp"
#include "orm/utils.hpp"
#include <concepts>

namespace espresso::orm {

template<auto field_ptr>
    requires ModelFieldPtr<decltype(field_ptr)>
class FilterField {
public:
    using Model = typename struct_field_ptr<decltype(field_ptr)>::Struct;
    using FieldType = typename struct_field_ptr<decltype(field_ptr)>::Field;

    FilterField() = default;
    ~FilterField() = default;

    template<typename T>
    Filter<Model> operator==(const T& other) const
    {
        return Filter<Model>::create(*this, "=", other);
    }
};

template<auto field_ptr>
static constexpr auto field_t = FilterField<field_ptr>();

}// namespace espresso::orm