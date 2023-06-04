//
// Created by michele on 08.01.23.
//

#ifndef ESPRESSO_SRC_HTTPMETHOD_H_
#define ESPRESSO_SRC_HTTPMETHOD_H_

#include <string>
namespace Espresso {

enum HTTPMethod {
  GET,
  POST,
  PUT,
  DELETE,
  HEAD,
  OPTIONS,
  PATCH,
  TRACE,
  CONNECT,
  ALL,
  UNDEFINED
};

std::string methodToString(HTTPMethod method);

HTTPMethod stringToMethod(std::string method);

}// namespace Espresso

#endif//ESPRESSO_SRC_HTTPMETHOD_H_
