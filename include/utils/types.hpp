#pragma once

#include "utils/tuple.hpp"

namespace espresso {

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

template<typename T>
using clean_type_t = std::remove_cvref_t<std::remove_pointer_t<T>>;


template<class T, template<class...> class Primary>
struct is_specialization_of : std::false_type {};

template<template<class...> class Primary, class... Args>
struct is_specialization_of<Primary<Args...>, Primary> : std::true_type {};

template<typename T, template<class...> class Primary>
inline constexpr bool is_specialization_of_v = is_specialization_of<T, Primary>::value;

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

template<typename T>
struct tuple_remove_pointers;

template<typename... Ts>
struct tuple_remove_pointers<Tuple<Ts...>> {
    using type = Tuple<clean_type_t<Ts>...>;
};

template<typename T>
using tuple_remove_pointers_t = tuple_remove_pointers<T>::type;

template<typename T>
T *remove_const(const T *ptr)
{
    return const_cast<T *>(ptr);
}

}// namespace espresso