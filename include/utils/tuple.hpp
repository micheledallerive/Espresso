#pragma once

#include "rfl/apply.hpp"
#include <rfl/Tuple.hpp>

namespace espresso {

template<typename... T>
using Tuple = rfl::Tuple<T...>;

}// namespace espresso