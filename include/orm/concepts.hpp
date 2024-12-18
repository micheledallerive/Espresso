#pragma once

#include "orm/model/field_property.hpp"
#include "orm/reflection/fields.hpp"
#include "orm/utils/types.hpp"
#include <concepts>
#include <rfl/field_names_t.hpp>
#include <rfl/internal/get_ith_field_from_fake_object.hpp>
#include <type_traits>

namespace espresso::orm {

template<typename Child>
class BaseModel;

using AllowedTypes = std::tuple<int, float, double, std::string, bool, char, long, long long>;

template<typename T>
class ForeignKey;

/**
 * Check whether all fields of the model have a type in "AllowedTypes"
 */
template<typename T, size_t _i>
consteval bool all_valid_types_impl()
{
    using Type = clean_type_t<decltype(refl::nth_field<T, _i>())>;
    if constexpr (has_type_v<Type, AllowedTypes>) {
        return true;
    }
    else if constexpr (is_specialization_of_v<Type, std::optional>) {
        return has_type_v<clean_type_t<typename Type::value_type>, AllowedTypes>;
    }
    else if constexpr (is_specialization_of_v<Type, ForeignKey>) {
        return not std::is_same_v<T, typename Type::Model> && (std::is_class_v<typename Type::Model>);//&& ModelConcept<typename Type::Model>);
    }
    else {
        return false;
    }
}
template<typename T>
consteval bool all_valid_types()
{
    return all_valid_types_impl<T, 0>();
    return []<size_t... _i>(std::index_sequence<_i...>) {
        return ((has_type_v<clean_type_t<decltype(refl::nth_field<T, _i>())>, AllowedTypes>) && ...);
    }(std::make_index_sequence<num_fields<T>()>{});
}

template<typename T>
concept ModelStructConcept =
        std::is_base_of_v<BaseModel<T>, T> && std::is_aggregate_v<T> && std::is_default_constructible_v<T> && all_valid_types<T>();

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
struct none_optional;

template<typename... Types>
struct none_optional<Tuple<Types...>> {
    static constexpr bool value = (not is_specialization_of_v<Types, std::optional> && ...);
};

template<typename T>
static constexpr bool none_optional_v = none_optional<T>::value;

template<typename T>
concept PrimaryKeyPropertyConcept = requires {
    typename T::ModelProperties;
    T::ModelProperties::primary_key;
} && is_specialization_of_v<std::remove_cvref_t<decltype(T::ModelProperties::primary_key)>, std::tuple> && none_optional_v<tuple_field_ptr_type_t<clean_type_t<decltype(T::ModelProperties::primary_key)>>>;

template<typename T>
concept ModelPropertiesConcept = PrimaryKeyPropertyConcept<T> && TableNamePropertyConcept<T>;

template<typename T>
concept ModelConcept = ModelStructConcept<T> && FieldPropertiesConcept<T> && ModelPropertiesConcept<T>;

template<typename T>
concept ModelFieldPtr = std::is_member_pointer_v<T> && std::is_member_object_pointer_v<T>;

}// namespace espresso::orm