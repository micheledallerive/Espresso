//
// Created by michele on 15.01.23.
//

#include <regex>
#include "PathRegex.h"
#include "utils.h"

namespace Espresso {

bool PathRegex::urlsMatch(const std::string &schema,
                          const std::string &url) {
  std::vector<std::string> schema_parts = split(schema, '/');
  std::vector<std::string> url_parts = split(url, '/');

  for (size_t i = 0; i < schema_parts.size() && i < url_parts.size(); i++) {
    if (schema_parts[i][0] == ':') continue;

    if (!urlPartMatch(schema_parts[i], url_parts[i])) return false;
  }
  return schema_parts.size() == url_parts.size() || (schema_parts.size() < url_parts.size() && schema_parts.back() == ".*")
  || (schema_parts.size() == url_parts.size() + 1 && schema_parts.back() == ".*");
}

bool PathRegex::urlPartMatch(const std::string &schemaPart,
                             const std::string &urlPart) {
  if (schemaPart.length() > 1 && schemaPart[0] == ':') return true;

  std::regex r(schemaPart);
  if (!std::regex_match(urlPart, r)) return false;
  return true;
}

std::string PathRegex::pathToRegex(const std::string &path) {
  std::vector<std::string> parts = split(path, '/');
  std::string regexPath;
  for (const std::string &part : parts) {
    std::string regex;
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
    regexPath += (regexPath.empty() ? "" : "/") + regex;
  }
  return "/" + regexPath;
}

} // Espresso