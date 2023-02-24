//
// Created by michele on 24.02.23.
//

#include "JSONLiteral.h"

namespace Espresso::JSON {

std::string JSONLiteral::toJSON() const {
  return '"' + value_ + '"';
}

} // JSON