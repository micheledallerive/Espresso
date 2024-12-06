#pragma once

#include <rfl/internal/get_ith_field_from_fake_object.hpp>
#include <rfl/internal/num_fields.hpp>
#include <tuple>
#include <type_traits>

namespace espresso::orm {

template<typename T, typename Tuple>
struct has_type;

template<typename T>
struct has_type<T, std::tuple<>> : std::false_type {};

template<typename T, typename U, typename... Ts>
struct has_type<T, std::tuple<U, Ts...>> : has_type<T, std::tuple<Ts...>> {};

template<typename T, typename... Ts>
struct has_type<T, std::tuple<T, Ts...>> : std::true_type {};

template<typename T, typename Tuple>
inline constexpr bool has_type_v = has_type<T, Tuple>::value;

/**
 * Check if all the fields of the struct T have type R
 */
template<typename R, typename T>
consteval bool all_same()
{
    return []<size_t... _i>(std::index_sequence<_i...>) {
        return (std::is_same_v<R, std::remove_cvref_t<std::remove_pointer_t<decltype(rfl::internal::get_ith_field_from_fake_object<T, _i>())>>> && ...);
    }(std::make_index_sequence<rfl::internal::num_fields<T>>{});
}

template<class T, template<class...> class Primary>
struct is_specialization_of : std::false_type {};

template<template<class...> class Primary, class... Args>
struct is_specialization_of<Primary<Args...>, Primary> : std::true_type {};

template<typename T, template<class...> class Primary>
inline constexpr bool is_specialization_of_v = is_specialization_of<T, Primary>::value;

template<typename T>
struct struct_field_ptr;

template<typename Model, typename FieldType>
struct struct_field_ptr<FieldType Model::*> {
    using Struct = Model;
    using Field = FieldType;
};

}// namespace espresso::orm