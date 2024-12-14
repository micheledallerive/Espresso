#pragma once

#include "utils/tuple.hpp"
#include <any>
#include <functional>
#include <string_view>
#include <vector>

namespace espresso::orm {

template<typename T>
concept SQLOperation = requires(const T t) {
    { t.compile() } -> std::same_as<std::string>;
};

template<typename T>
concept SQLCompilerQueryConcept = SQLOperation<T> && requires(T t) {
    { t.filter(std::declval<std::string_view>()) } -> std::same_as<T&>;
    { t.order_by(std::declval<std::string_view>()) } -> std::same_as<T&>;
};

template<typename T>
concept SQLCompilerInsertConcept = SQLOperation<T> && requires(T t) {
    { t.insert(std::declval<std::string_view>(), std::declval<std::any>()) } -> std::same_as<T&>;
};

template<typename T>
concept SQLCompilerDeleteConcept = SQLOperation<T> && requires(T t) {
    { t.filter(std::declval<std::string_view>(), std::declval<std::any>()) } -> std::same_as<T&>;
};

template<typename T>
concept SQLCompilerConcept = requires {
    typename T::Query;
    typename T::Insert;
    typename T::Delete;
} && SQLCompilerQueryConcept<typename T::Query> && SQLCompilerInsertConcept<typename T::Insert> && SQLCompilerDeleteConcept<typename T::Delete>;

template<typename T>
concept DBInstanceConcept = requires(T t) {
    { t.execute_query(std::declval<std::string_view>()) } -> std::same_as<void>;
    { t.execute_query(std::declval<std::string_view>(), std::declval<std::function<void(const Tuple<std::any>&)>&&>()) } -> std::same_as<size_t>;
    { t.start_transaction() } -> std::same_as<void>;
    { t.commit() } -> std::same_as<void>;
    { t.rollback() } -> std::same_as<void>;
    typename T::Compiler;
} && SQLCompilerConcept<typename T::Compiler>;

}// namespace espresso::orm