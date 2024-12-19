#pragma once

namespace espresso {

template<typename... Args>
using Tuple = std::tuple<Args...>;

using std::tuple_size_v;
using std::tuple_element_t;
using std::apply;
using std::make_tuple;

}// namespace espresso