#pragma once

#include "orm/model/field_property.hpp"
#include "orm/utils.hpp"
#include <concepts>
#include <rfl/field_names_t.hpp>
#include <rfl/internal/get_ith_field_from_fake_object.hpp>

namespace espresso::orm {

template<typename Child>
class BaseModel;

using AllowedTypes = std::tuple<int, float, double, std::string, bool, char, long, long long>;

/**
 * Check whether all fields of the model have a type in "AllowedTypes"
 */
template<typename T>
consteval bool all_valid_types()
{
    return []<size_t... _i>(std::index_sequence<_i...>) {
        return (has_type_v<std::remove_cvref_t<std::remove_pointer_t<decltype(rfl::internal::get_ith_field_from_fake_object<T, _i>())>>, AllowedTypes> && ...);
    }(std::make_index_sequence<rfl::internal::num_fields<T>>{});
}

template<typename T>
struct contains_double_underscore;

template<auto Head, auto... Tail>
struct contains_double_underscore<rfl::Literal<Head, Tail...>> {
    static constexpr bool value =
            (Head.string_view().find("__") != std::string_view::npos) || contains_double_underscore<rfl::Literal<Tail...>>::value;
};

// Base case: No more elements in the pack
template<>
struct contains_double_underscore<rfl::Literal<>> {
    static constexpr bool value = false;
};

// Helper for easier usage
template<typename T>
constexpr bool contains_double_underscore_v = contains_double_underscore<T>::value;

template<typename T>
concept ModelStructConcept =
        std::is_base_of_v<BaseModel<T>, T> && std::is_aggregate_v<T> && std::is_default_constructible_v<T> && all_valid_types<T>() && !contains_double_underscore_v<rfl::field_names_t<T>>;

template<typename T>
concept HasFieldProperties = requires {
    typename T::FieldProperties;
};

template<typename T>
concept HasModelProperties = requires {
    typename T::ModelProperties;
};
template<typename T>
concept HasTableName = HasModelProperties<T> && requires {
    T::ModelProperties::table_name;
};

/**
 * Require the innerclass FieldProperties to be present and all fields of the struct to have the same FieldProperties.
 */
template<typename T>
concept PresentFieldPropertiesConcept = HasFieldProperties<T> && all_same<FieldPropertyList, typename T::FieldProperties>();

/**
 * Require the innerclass FieldProperties to be absent.
 */
template<typename T>
concept AbsentFieldPropertiesConcept = not HasFieldProperties<T>;

// FieldPropertiesConcept must be:
// Either T::FieldProperties exists and T::FieldProperties is aggregate
// Or T::FieldProperties does not exist
template<typename T>
concept FieldPropertiesConcept = PresentFieldPropertiesConcept<T> || AbsentFieldPropertiesConcept<T>;

template<typename T>
concept TableNamePropertyConcept = std::same_as<std::string_view, std::remove_cvref_t<decltype(T::ModelProperties::table_name)>> || not requires {
    { T::ModelProperties::table_name };
};
template<typename T>
concept PrimaryKeyPropertyConcept = requires {
    typename T::ModelProperties;
    T::ModelProperties::primary_key;
} && is_specialization_of_v<std::remove_cvref_t<decltype(T::ModelProperties::primary_key)>, std::tuple>;

template<typename T>
concept ModelPropertiesConcept = requires {
    typename T::ModelProperties;
    T::ModelProperties::primary_key;
} && TableNamePropertyConcept<T>;

template<typename T>
concept ModelConcept = ModelStructConcept<T> && FieldPropertiesConcept<T> && ModelPropertiesConcept<T>;

template<typename T>
concept ModelFieldPtr = std::is_member_pointer_v<T> && std::is_member_object_pointer_v<T>;

}// namespace espresso::orm