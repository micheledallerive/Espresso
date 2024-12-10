#pragma once

#include <rfl/Tuple.hpp>

namespace espresso {

template<typename... T>
using Tuple = rfl::Tuple<T...>;

}