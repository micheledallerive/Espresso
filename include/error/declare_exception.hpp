#pragma once

namespace espresso {

#define DECLARE_EXCEPTION(name, message)        \
    class name : public std::runtime_error {    \
    public:                                     \
        name() : std::runtime_error(message) {} \
    };

}