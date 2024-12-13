#pragma once

#include "orm/model/meta_model.hpp"
#include "orm/utils/stringify.hpp"
#include <string>

namespace espresso::orm {
template<auto field_ptr>
    requires ModelFieldPtr<decltype(field_ptr)>
struct FilterField;

class Filter {
private:
    std::string m_str;

    template<auto field_ptr>
    constexpr static std::string column_name() {
        using Model = typename struct_field_ptr<decltype(field_ptr)>::Struct;
        return MetaModel<Model>::compile_time::table_name() + "." + MetaModel<Model>::column_name(get_field_name_str<field_ptr>());
    }
public:
    constexpr Filter(const std::string& left, const std::string& op, const std::string& right) : m_str{left + " " + op + " " + right} {}

    [[nodiscard]] const std::string& str() const
    {
        return m_str;
    }

    template<auto field_ptr1, auto field_ptr2>
//        requires std::is_same_v<typename struct_field_ptr<decltype(field_ptr1)>::Field,
//                                typename struct_field_ptr<decltype(field_ptr2)>::Field>
    static constexpr Filter create(const FilterField<field_ptr1>& field1, std::string op, const FilterField<field_ptr2>& field2)
    {
        return Filter{column_name<field_ptr1>(), std::move(op), column_name<field_ptr2>()};
    }

    template<auto field_ptr>
    static constexpr Filter create(const FilterField<field_ptr>& field, std::string op, const struct_field_ptr<decltype(field_ptr)>::Field& value)
    {
        using Model = typename struct_field_ptr<decltype(field_ptr)>::Struct;
        using FieldType = typename struct_field_ptr<decltype(field_ptr)>::Field;
        return Filter{column_name<field_ptr>(), std::move(op), quote_stringify<FieldType>::to_string(value)};
    }

    template<auto field_ptr>
    static constexpr Filter create(const FilterField<field_ptr>& field, std::string op, const std::optional<typename struct_field_ptr<decltype(field_ptr)>::Field>& value)
    {
        if (value.has_value()) {
            return create(field, std::move(op), value.value());
        }
        using Model = typename struct_field_ptr<decltype(field_ptr)>::Struct;
        static constexpr auto equals = "=";
        static constexpr auto not_equals = "!=";
        if (op != equals && op != not_equals) throw std::runtime_error("Null comparison can only be equal or not equal");
        return Filter{column_name<field_ptr>, op == equals ? "IS" : "IS NOT", "NULL"};
    }
};

}// namespace espresso::orm