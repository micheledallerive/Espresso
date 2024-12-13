#pragma once

#include <stdexcept>

namespace espresso::orm {

#define DECLARE_EXCEPTION(name, message)        \
    class name : public std::runtime_error {    \
    public:                                     \
        name() : std::runtime_error(message) {} \
    };

DECLARE_EXCEPTION(ObjectDoesNotExist, "Object does not exist")

}// namespace espresso::orm