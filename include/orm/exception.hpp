#pragma once

#include "error/declare_exception.hpp"
#include <stdexcept>

namespace espresso::orm {

DECLARE_EXCEPTION(ObjectDoesNotExist, "Object does not exist")

}// namespace espresso::orm