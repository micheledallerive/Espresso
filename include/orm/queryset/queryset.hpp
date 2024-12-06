#pragma once

#include "orm/queryset/filter.hpp"
#include "orm/queryset/filter_field.hpp"
#include <rfl/field_type.hpp>
#include <rfl/internal/get_field_names.hpp>

namespace espresso::orm {

template<typename Model>
class QuerySet {
private:
    template<auto name>
    using field_type = rfl::field_type_t<name, Model>;

    std::vector<std::string> m_filters;

public:
    QuerySet() = default;

    QuerySet& filter(const Filter& filter)
    {
        m_filters.push_back(filter.to_string());
        return *this;
    }
};

}// namespace espresso::orm