//
// Created by michele on 09.01.23.
//

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

std::string getUTCDate() {
  char out[30];
  time_t t = time(nullptr);
  struct tm *tmp = gmtime(&t);
  const char *format = "%a, %d %b %Y %T GMT";
  if (!tmp)
    return "";
  if (strftime(out, sizeof(out), format, tmp) == 0)
    return "";
  return out;
}

std::string getAbsolutePath(const std::string &path) {
  return path[0] == '/'
             ? path
             : (any_cast<std::string>(Espresso::server_settings["BASE_PATH"]) +
                path);
}

void splitListOfPairs(
    const std::string &line, char delim, char pairDelim,
    std::function<void(const std::string &, const std::string &)> callback) {
  std::istringstream iss(line);
  std::string values;

  while (std::getline(iss, values, delim)) {
    if (values.empty())
      continue;

    std::string key, value;

    auto pos = values.find(pairDelim);
    if (pos == std::string::npos || pos == 0)
      continue;

    key = values.substr(0, pos);
    value = values.substr(pos + 1);
    if (key.empty())
      continue;

    callback(key, value);
  }
}

} // namespace Espresso
