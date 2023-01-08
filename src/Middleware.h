//
// Created by michele on 08.01.23.
//

#ifndef ESPRESSO_SRC_MIDDLEWARE_H_
#define ESPRESSO_SRC_MIDDLEWARE_H_

#include "HTTPRequest.h"
#include "HTTPResponse.h"

namespace Espresso {

using Middleware = std::function<void(Request&, Response&)>;

} // Espresso

#endif //ESPRESSO_SRC_MIDDLEWARE_H_
