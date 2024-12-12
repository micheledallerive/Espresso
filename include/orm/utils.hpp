#pragma once

#include "utils/tuple.hpp"
#include <any>
#include <rfl/internal/get_ith_field_from_fake_object.hpp>
#include <rfl/internal/num_fields.hpp>
#include <rfl/named_tuple_t.hpp>
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

template<typename T>
using model_to_tuple_t = typename rfl::named_tuple_t<T>::Values;

//template<typename RflTuple>
//struct rfl_tuple_to_tuple;
//
//template<typename... T>
//struct rfl_tuple_to_tuple<rfl::Tuple<T...>> {
//    using type = std::tuple<typename T::Type...>;
//};
//
//template<typename NamedTuple>
//struct rfl_namedtuple_to_tuple {
//    using type = typename rfl_tuple_to_tuple<typename NamedTuple::Fields>::type;
//};
//
//template<typename NamedTuple>
//using rfl_namedtuple_to_tuple_t = typename rfl_namedtuple_to_tuple<NamedTuple>::type;

template<typename T, typename Tuple>
T construct_from_tuple(const Tuple& tuple)
{
    return rfl::apply([](auto&&... args) {
        return T{{},args...};
    },
                      tuple);
}

template<typename T>
struct stringify {
    static std::string to_string(const T& t)
    {
        return std::to_string(t);
    }
};

template<>
struct stringify<std::string> {
    static std::string to_string(const std::string& t)
    {
        return t;
    }
};

template<typename T>
T any_cast_if_present(const std::any &a) {
    if (!a.has_value()) {
        return {};
    }
    return std::any_cast<T>(a);
}

template<typename... Types>
Tuple<Types...> vector_to_tuple(const std::vector<std::any>& vec)
{
    Tuple<Types...> t;
    size_t i = 0;
    [&]<size_t... _i>(std::index_sequence<_i...>) {
        ((std::get<_i>(t) = any_cast_if_present<rfl::internal::nth_element_t<_i, Types...>>(vec[i++])), ...);
    }(std::make_index_sequence<sizeof...(Types)>{});
    return t;
}

}// namespace espresso::orm