//
// Created by michele on 09.01.23.
//

#ifndef ESPRESSO_SRC_UTILS_H_
#define ESPRESSO_SRC_UTILS_H_

#include <vector>
#include <string>
#include <functional>

namespace Espresso {
std::vector<std::string> split(const std::string &s, char delim);
std::vector<std::string> split(const std::string &s, const std::string &delim);

std::string getUTCDate();

std::string getAbsolutePath(const std::string &path);

void splitListOfPairs(const std::string &line,
                      char delim,
                      char pairDelim,
                      std::function<void(const std::string &,
                                         const std::string &)> callback);

std::string urlDecode(const std::string &str);

}

#endif //ESPRESSO_SRC_UTILS_H_
