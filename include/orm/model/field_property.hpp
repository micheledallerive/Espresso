#pragma once

#include <string>
#include <variant>
#include <vector>

namespace espresso::orm {

enum class Property {
    MAX_LENGTH,
    COLUMN_NAME
};

struct FieldProperty {
    Property property;
    std::variant<std::string, bool> value;
};

struct FieldPropertyList {
    std::vector<FieldProperty> m_properties{};
    FieldPropertyList() : m_properties{} {}
    FieldPropertyList(std::initializer_list<FieldProperty> props) : m_properties{props} {}
    template<typename... Args>
    explicit FieldPropertyList(Args&&... args) : FieldPropertyList({std::forward<Args>(args)...})
    {}
    [[nodiscard]] const std::vector<FieldProperty>& properties() const { return m_properties; }
};

FieldProperty max_length(size_t max_len)
{
    return FieldProperty{Property::MAX_LENGTH, std::to_string(max_len)};
}

FieldProperty column_name(const std::string& col_name)
{
    return FieldProperty{Property::COLUMN_NAME, col_name};
}

}// namespace espresso::orm
