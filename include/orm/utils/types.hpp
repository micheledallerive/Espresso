#pragma once

#include "orm/reflection/fields.hpp"
#include "orm/utils/anycast.hpp"
#include "utils/tuple.hpp"
#include "utils/types.hpp"
#include <tuple>
#include <type_traits>

namespace espresso::orm {
/**
 * Check if all the fields of the struct T have type R
 */
template<typename R, typename T>
consteval bool all_same()
{
    return []<size_t... _i>(std::index_sequence<_i...>) {
        return (std::is_same_v<R, clean_type_t<decltype(refl::nth_field<T, _i>())>> && ...);
    }(std::make_index_sequence<refl::num_fields<T>()>{});
}

template<typename T>
struct struct_field_ptr;

template<typename Model, typename FieldType>
struct struct_field_ptr<FieldType Model::*> {
    using Struct = Model;
    using Field = FieldType;
};

template<typename T>
using model_to_tuple_t = refl::FieldTypes<T>;

template<typename T, typename Tuple>
T construct_from_tuple(const Tuple& tuple)
{
    return apply([](auto&&... args) {
        return T{{}, args...};
    },
                 tuple);
}

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
                                              Index + 1 + (tuple_size_v<typename T::Compound> > 0 ? tuple_size_v<typename T::Compound> - 1 : 0),
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
using tuple_field_ptr_type_t = typename tuple_field_ptr_type<std::remove_cvref_t<T>>::type;

template<typename T>
struct is_complete_helper {
    template<typename U>
    static auto test(U*) -> std::integral_constant<bool, sizeof(U) == sizeof(U)>;
    static auto test(...) -> std::false_type;
    using type = decltype(test((T*) 0));
};

template<typename T>
struct is_complete : is_complete_helper<T>::type {};

template<typename T>
static constexpr bool is_complete_v = is_complete<T>::value;

}// namespace espresso::orm