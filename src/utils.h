//
// Created by michele on 09.01.23.
//

#ifndef ESPRESSO_SRC_UTILS_H_
#define ESPRESSO_SRC_UTILS_H_

#include <vector>
#include <string>

namespace Espresso {
std::vector<std::string> split(const std::string &s, char delim);

bool urls_match(const std::string &schema,
                const std::string &url,
                bool ignore_params = true);

std::string getUTCDate();

std::string getAbsolutePath(const std::string &path);
}

#endif //ESPRESSO_SRC_UTILS_H_
