//
// Created by michele on 15.01.23.
//

#include <regex>
#include "PathRegex.h"
#include "utils.h"

namespace Espresso {

bool PathRegex::urlsMatch(const std::string &schema,
                          const std::string &url) {
  std::regex regex(schema);
  return std::regex_match(url, regex);
}

bool PathRegex::retrieveParams(const std::string &schema,
                               const std::string &url,
                               std::map<std::string, std::string> &params) {
  std::regex regex(schema);
  std::smatch match;

  if (std::regex_match(url, match, regex)) {
    for (int i = 1; i < match.size(); ++i) {
      std::string param = match[i];
      std::string key = schema.substr(schema.find(':') + 1,
                                      schema.find(')') - schema.find(':') - 1);
      params[key] = param;
    }
    return true;
  }
  return false;
}

std::string PathRegex::pathToRegex(const std::string &path) {
  std::vector<std::string> parts = split(path, '/');
  std::string regexPath;
  for (const auto &part : parts) {
    if (part.empty()) continue;

    std::string regex;
    if (part[0] == ':') {
      regex = "(?<p:" + part.substr(1) + "[^/]+)";
    } else {
      for (const char &c : part) {
        switch (c) {
          case '*':regex += ".*";
            break;
          case '?':regex += ".?";
            break;
          case '+':regex += ".+";
            break;
          default:regex += c;
        }
      }
    }
    regexPath += "/" + regex;
  }
  return regexPath;
}

} // Espresso