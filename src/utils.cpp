//
// Created by michele on 09.01.23.
//

#include <algorithm>
#include <vector>
#include <string>
#include <sstream>

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
  if (schema_parts.size() != url_parts.size()) return false;

  for (int i = 0; i < schema_parts.size(); ++i) {
    if (schema_parts[i] == url_parts[i]) continue;
    if (schema_parts[i] == "*") continue;
    if (schema_parts[i][0] == ':' && ignore_params) continue;
    return false;
  }
  return true;
}
