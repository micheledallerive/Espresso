#pragma once

#include <string>
#include <variant>
#include <vector>

namespace espresso::orm {

struct FieldProperty {
    std::string name;
    std::variant<std::string, bool> value;
};

struct FieldPropertyList {
    std::vector<FieldProperty> properties;
    FieldPropertyList() : properties{} {}
    template<typename... Args>
    FieldPropertyList(Args&&... args) : properties{std::forward<Args>(args)...} {}
};

FieldProperty max_length(size_t max_len)
{
    return FieldProperty{"max_length", std::to_string(max_len)};
}

FieldProperty required()
{
    return FieldProperty{"required", true};
}

}// namespace espresso::orm
