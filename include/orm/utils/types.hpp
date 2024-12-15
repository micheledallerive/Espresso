#pragma once

#include "orm/utils/anycast.hpp"
#include "rfl/to_view.hpp"
#include "utils/tuple.hpp"
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
template<typename T, typename Tuple>
T construct_from_tuple(const Tuple& tuple)
{
    return rfl::apply([](auto&&... args) {
        return T{{}, args...};
    },
                      tuple);
}

// Concatenate two index sequences
template<typename Seq1, typename Seq2>
struct index_sequence_cat;

template<std::size_t... I1, std::size_t... I2>
struct index_sequence_cat<std::index_sequence<I1...>, std::index_sequence<I2...>> {
    using type = std::index_sequence<I1..., I2...>;
};

// Helper alias for simplicity
template<typename Seq1, typename Seq2>
using index_sequence_cat_t = typename index_sequence_cat<Seq1, Seq2>::type;

// Recursive implementation of make_index_sequence_skip_tuple
template<std::size_t Index, typename... Types>
struct make_index_sequence_skip_tuple_impl;

// Base case: empty pack
template<std::size_t Index>
struct make_index_sequence_skip_tuple_impl<Index> {
    using type = std::index_sequence<>;
};

// Case 1: Non-tuple type
template<std::size_t Index, typename T, typename... Rest>
struct make_index_sequence_skip_tuple_impl<Index, T, Rest...> {
    using type = index_sequence_cat_t<
            std::index_sequence<Index>,
            typename make_index_sequence_skip_tuple_impl<Index + 1, Rest...>::type>;
};

// Case 2: Compound type, i.e. a type with T::Compound
template<std::size_t Index, typename T, typename... Rest>
    requires requires {
        typename T::Compound;
    }
struct make_index_sequence_skip_tuple_impl<Index, T, Rest...> {
    using type = index_sequence_cat_t<std::index_sequence<Index>,
                                      typename make_index_sequence_skip_tuple_impl<
                                              Index + 1 + (rfl::tuple_size_v<typename T::Compound> > 0 ? rfl::tuple_size_v<typename T::Compound> - 1 : 0),
                                              Rest...>::type>;
};

// Helper alias
template<typename... Types>
using make_index_sequence_skip_compound = typename make_index_sequence_skip_tuple_impl<0, Types...>::type;

template<typename T>
struct tuple_field_ptr_type;
template<typename... Ptrs>
struct tuple_field_ptr_type<std::tuple<Ptrs...>> {
    using type = Tuple<std::remove_cvref_t<typename struct_field_ptr<Ptrs>::Field>...>;
};

template<typename T>
using tuple_field_ptr_type_t = tuple_field_ptr_type<std::remove_cvref_t<T>>::type;

}// namespace espresso::orm