#pragma once

#include "orm/concepts.hpp"
#include "orm/queryset/filter.hpp"
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

#define FILTER_FIELD_OPERATOR(op)                        \
    template<typename T>                                 \
    Filter operator op(const T & other) const     \
    {                                                    \
        return Filter::create(*this, #op, other); \
    }

    FILTER_FIELD_OPERATOR(==)
    FILTER_FIELD_OPERATOR(!=)
    FILTER_FIELD_OPERATOR(>)
    FILTER_FIELD_OPERATOR(<)
    FILTER_FIELD_OPERATOR(>=)
    FILTER_FIELD_OPERATOR(<=)
};

template<auto field_ptr>
static constexpr auto field_t = FilterField<field_ptr>();

}// namespace espresso::orm