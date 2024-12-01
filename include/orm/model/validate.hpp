#pragma once

#include "orm/utils.hpp"
#include <concepts>
#include <rfl/internal/get_ith_field_from_fake_object.hpp>

namespace espresso::orm {

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
concept ModelStructConcept = std::is_aggregate_v<T> && std::is_default_constructible_v<T> && all_valid_types<T>();

/**
 * Require the innerclass FieldProperties to be present and all fields of the struct to have the same FieldProperties.
 */
template<typename T>
concept PresentFieldPropertiesConcept = requires {
    typename T::FieldProperties;
} && all_same<FieldPropertyList, typename T::FieldProperties>();

/**
 * Require the innerclass FieldProperties to be absent.
 */
template<typename T>
concept AbsentFieldPropertiesConcept = not requires {
    typename T::FieldProperties;
};

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

}// namespace espresso::orm