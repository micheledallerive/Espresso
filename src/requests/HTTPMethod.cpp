//
// Created by michele on 08.01.23.
//

#include "requests/HTTPMethod.h"

namespace Espresso {

std::string methodToString(HTTPMethod method) {
  switch (method) {
  case GET:
    return "GET";
  case POST:
    return "POST";
  case PUT:
    return "PUT";
  case DELETE:
    return "DELETE";
  case HEAD:
    return "HEAD";
  case OPTIONS:
    return "OPTIONS";
  case PATCH:
    return "PATCH";
  case TRACE:
    return "TRACE";
  case CONNECT:
    return "CONNECT";
  case ALL:
    return "ALL";
  default:
    return "UNDEFINED";
  }
}

HTTPMethod stringToMethod(std::string method) {
  // make method uppercase
  std::transform(method.begin(), method.end(), method.begin(), ::toupper);
  if (method == "GET") {
    return GET;
  } else if (method == "POST") {
    return POST;
  } else if (method == "PUT") {
    return PUT;
  } else if (method == "DELETE") {
    return DELETE;
  } else if (method == "HEAD") {
    return HEAD;
  } else if (method == "OPTIONS") {
    return OPTIONS;
  } else if (method == "PATCH") {
    return PATCH;
  } else if (method == "TRACE") {
    return TRACE;
  } else if (method == "CONNECT") {
    return CONNECT;
  } else if (method == "ALL") {
    return ALL;
  } else {
    return UNDEFINED;
  }
}

}// namespace Espresso