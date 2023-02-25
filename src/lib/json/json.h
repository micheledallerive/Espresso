//
// Created by michele on 25.02.23.
//

#ifndef ESPRESSO_SRC_LIB_JSON_JSON_H_
#define ESPRESSO_SRC_LIB_JSON_JSON_H_
#include "JSONBase.h"
#include <string>

namespace Espresso::JSON {

JSONBase *parse(const std::string &json);

} // JSON

#endif //ESPRESSO_SRC_LIB_JSON_JSON_H_
