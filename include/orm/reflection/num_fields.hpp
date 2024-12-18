#pragma once

#include <algorithm>
#include <cstddef>

namespace espresso::orm::refl {

namespace internal {

template<typename T>
struct base_castable {
    base_castable(std::size_t);
    template<typename Base>
        requires(std::is_base_of_v<std::remove_cvref_t<Base>, std::remove_cv_t<T>> && !std::is_same_v<std::remove_cvref_t<Base>, std::remove_cv_t<T>>)
    constexpr
    operator Base&() const noexcept;
};

struct any_castable {
    any_castable(std::size_t);
    template<typename T>
    constexpr operator T() const noexcept;
};

// Creating a struct saves compilation time as T is shared
template<typename T>
struct FieldsCounter {

    template<std::size_t n, std::size_t tot_args>
    static consteval bool has_n_base_param()
    {
        constexpr size_t rhs = tot_args >= n ? tot_args - n : 0;
        return []<std::size_t... l, std::size_t... r>(std::index_sequence<l...>,
                                                      std::index_sequence<r...>) {
            return requires { T{base_castable<T>(l)..., any_castable(r)...}; };
        }(std::make_index_sequence<n>(), std::make_index_sequence<rhs>());
    }

    template<std::size_t tot_args, std::size_t index = 1>
    static consteval std::size_t num_base_param()
    {
        if constexpr (!has_n_base_param<index, tot_args>()) {
            return index - 1;
        }
        else {
            return num_base_param<tot_args, index + 1>();
        }
    }

    template<size_t N>
    static consteval size_t num_fields()
    {
        constexpr bool constructible = []<size_t... Is>(std::index_sequence<Is...>) {
            return requires { T{any_castable(Is)...}; };
        }(std::make_index_sequence<N>());

        if constexpr (!constructible) {
            return N - 1;
        }
        else {
            return num_fields<N + 1>();
        }
    }
};

}// namespace internal

template<typename T>
static consteval size_t num_fields()
{
    constexpr size_t tot_fields = internal::FieldsCounter<T>::template num_fields<0>();
    return tot_fields - internal::FieldsCounter<T>::template num_base_param<tot_fields>();
}

}// namespace espresso::orm::refl