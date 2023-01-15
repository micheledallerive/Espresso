//
// Created by michele on 15.01.23.
//

#ifndef ESPRESSO_SRC_PATHREGEX_H_
#define ESPRESSO_SRC_PATHREGEX_H_

#include <string>
#include <vector>

namespace Espresso {

class PathRegex {
 public:
  static bool urlsMatch(const std::string &schema,
                        const std::string &url);
  static std::string pathToRegex(const std::string &path);
  static bool urlPartMatch(const std::string &schemaPart,
                           const std::string &urlPart);
};

} // Espresso

#endif //ESPRESSO_SRC_PATHREGEX_H_
