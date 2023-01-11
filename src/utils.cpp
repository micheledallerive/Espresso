//
// Created by michele on 09.01.23.
//

#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "Server.h"

namespace Espresso {
std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  std::istringstream iss(s);
  std::string item;
  while (std::getline(iss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

/**
 * Checks if two URLs match.
 * @param schema the schema url. (e.g. /api/\*, /api/:id, /api/test)
 * @param url the url to match. (e.g. /api/test, /api/5, etc)
 * @param ignore_params weather to ignore the parameters or not. (e.g. /api/:id and /api/5 will match if ignore_params is true)
 * @return
 */
bool urls_match(const std::string &schema,
                const std::string &url,
                bool ignore_params = true) {
  std::vector<std::string> schema_parts = split(schema, '/');
  std::vector<std::string> url_parts = split(url, '/');
  for (int i = 0; i < url_parts.size(); ++i) {
    if (i >= schema_parts.size()) return false;
    if (schema_parts[i] == url_parts[i]) continue;
    if (schema_parts[i] == "*") {
      if (i == schema_parts.size() - 1) return true;
      continue;
    }
    if (schema_parts[i][0] == ':' && ignore_params
        && url_parts[i].length() > 0)
      continue;
    return false;
  }
  return (schema_parts.size() <= url_parts.size()
      || (schema_parts.size() == url_parts.size() + 1
          && schema_parts.back() == "*"));
}

std::string getUTCDate() {
  char out[30];
  time_t t = time(nullptr);
  struct tm *tmp = gmtime(&t);
  const char *format = "%a, %d %b %y %T GMT";
  if (!tmp) return "";
  if (strftime(out, sizeof(out), format, tmp) == 0) return "";
  return out;
}

std::string getAbsolutePath(const std::string &path) {
  return path[0] == '/' ? path : (
      any_cast<std::string>(Espresso::server_settings["BASE_PATH"]) + path);
}

void splitListOfPairs(const std::string &line,
                      char delim,
                      char pairDelim,
                      std::function<void(const std::string &,
                                         const std::string &)> callback) {
  std::istringstream iss(line);
  std::string values;

  while (std::getline(iss, values, delim)) {
    if (values.empty()) continue;

    std::string key, value;

    auto pos = values.find(pairDelim);
    if (pos == std::string::npos || pos == 0)
      continue;

    key = values.substr(0, pos);
    value = values.substr(pos + 1);
    if (key.empty()) continue;

    callback(key, value);
  }
}

}
