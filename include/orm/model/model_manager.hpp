#pragma once

#include "orm/concepts.hpp"

namespace espresso::orm {

namespace internal {

template<typename T>
void create_model_table() {

}

}

template<typename T>
    requires ModelConcept<T>
static void register_model()
{
}

template<typename... Ts>
static void register_models()
{
    (register_model<Ts>(), ...);
}

}// namespace espresso::orm