#pragma once

#include "orm/concepts.hpp"
#include "orm/model/field_property.hpp"
#include "orm/reflection/field_name.hpp"
#include <map>
#include <rfl.hpp>

namespace espresso::orm {

/**
 * Singleton model meta information
 */
template<typename Model>
class MetaModel {
private:
    std::map<std::string, FieldPropertyList> m_field_properties;

    void setup_field_properties()
    {
        using FP = typename Model::FieldProperties;
        FP properties{};
        auto view = rfl::to_view(properties);
        view.apply([&](const auto& f) {
            std::string name{f.name()};
            FieldPropertyList val = *f.value();
            m_field_properties.emplace(name, std::move(val));
        });
    }

    MetaModel()
    {
        if constexpr (HasFieldProperties<Model>) {
            setup_field_properties();
        }
    }

public:
    static MetaModel& instance()
    {
        static MetaModel instance;
        return instance;
    }
    ~MetaModel() = default;

    std::optional<FieldProperty> find_property(const std::string & field_name, Property property)
    {
        auto it = m_field_properties.find(field_name);
        if (it == m_field_properties.end()) {
            return std::nullopt;
        }

        auto& field_properties = it->second;
        for (auto& field_property : field_properties.properties()) {
            if (field_property.property == property) {
                return field_property;
            }
        }
        return std::nullopt;

    }

    template<auto field_ptr>
    std::optional<FieldProperty> find_property(Property property)
    {
        auto field_name = get_field_name_str<field_ptr>();
        return find_property(field_name, property);
    }

    static std::string column_name(const std::string &field_name) noexcept
    {
        MetaModel<Model> me = instance();
        auto prop = me.find_property(field_name, Property::COLUMN_NAME);
        if (prop.has_value()) {
            return std::get<std::string>(prop.value().value);
        }
        return field_name;
    }

    struct compile_time {
        [[nodiscard]] static constexpr std::string table_name() noexcept
        {
            if constexpr (HasTableName<Model>) {
                return std::string(Model::ModelProperties::table_name);
            }
            else {
                return rfl::type_name_t<Model>().str();
            }
        }
    };
};

}// namespace espresso::orm