//
// Created by michele on 15.01.23.
//

#ifndef ESPRESSO_SRC_PATHREGEX_H_
#define ESPRESSO_SRC_PATHREGEX_H_

#include "route.h"
#include <string>
#include <vector>

namespace Espresso {

class PathRegex {
public:
  static bool urlsMatch(const std::string &schema,
                        const std::string &url);
  static std::string pathToRegex(const std::string &path);
  static bool retrieveParams(const Route &route,
                             const std::string &url,
                             std::unordered_map<std::string, std::string> &params);
};

}// namespace Espresso

#endif//ESPRESSO_SRC_PATHREGEX_H_
