//
// Created by michele on 24.02.23.
//

#include "JSONBoolean.h"

namespace Espresso::JSON {

std::string JSONBoolean::toJSON() const {
  return value_ ? "true" : "false";
}

}