#pragma once

#include "error/declare_exception.hpp"

namespace espresso {

DECLARE_EXCEPTION(TimeoutException, "I/O timeout occurred");

}