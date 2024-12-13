#pragma once

#include "orm/model/meta_model.hpp"
#include "orm/utils/stringify.hpp"
#include <string>

namespace espresso::orm {
template<auto field_ptr>
    requires ModelFieldPtr<decltype(field_ptr)>
struct FilterField;

template<typename Model>
class Filter {
private:
    std::string m_str;

public:
    Filter(std::string left, std::string op, std::string right) : m_str{left + " " + op + " \"" + right + "\""} {}

    [[nodiscard]] const std::string& str() const
    {
        return m_str;
    }

    template<auto field_ptr1, auto field_ptr2>
        requires std::is_same_v<typename struct_field_ptr<decltype(field_ptr1)>::Field,
                                typename struct_field_ptr<decltype(field_ptr2)>::Field>
            && std::is_same_v<typename struct_field_ptr<decltype(field_ptr1)>::Struct, Model>
            && std::is_same_v<typename struct_field_ptr<decltype(field_ptr2)>::Struct, Model>
    static constexpr Filter create(FilterField<field_ptr1> field1, std::string op, FilterField<field_ptr2> field2)
    {
        return Filter{MetaModel<Model>::column_name(get_field_name_str<field_ptr1>()), std::move(op), MetaModel<Model>::column_name(get_field_name_str(field_ptr2))};
    }

    template<auto field_ptr>
        requires std::is_same_v<typename struct_field_ptr<decltype(field_ptr)>::Struct, Model>
    static constexpr Filter create(FilterField<field_ptr> field, std::string op, const struct_field_ptr<decltype(field_ptr)>::Field& value)
    {
        using FieldType = typename struct_field_ptr<decltype(field_ptr)>::Field;
        return Filter{MetaModel<Model>::column_name(get_field_name_str<field_ptr>()), std::move(op), stringify<FieldType>::to_string(value)};
    }
};

}// namespace espresso::orm