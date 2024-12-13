#pragma once

#include <stdexcept>

namespace espresso::orm {

class ObjectDoesNotExist : public std::runtime_error {
public:
    ObjectDoesNotExist() : std::runtime_error("Object does not exist") {}
};

}// namespace espresso::orm