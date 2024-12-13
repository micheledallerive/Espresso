#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <algorithm>

namespace espresso {

[[maybe_unused]] static bool equals_case_insensitive(std::string_view s1, std::string_view s2)
{
    return s1.size() == s2.size() && std::equal(s1.begin(), s1.end(), s2.begin(), s2.end(), [](char a, char b) {
               return std::tolower(a) == std::tolower(b);
           });
}

[[maybe_unused]] static std::string concatenate(const std::vector<std::string> &strings, const std::string &separator)
{
    if (strings.size() == 1) return strings[0];
    std::string result;
    for (size_t i = 0; i < strings.size(); ++i) {
        result += strings[i];
        if (i != strings.size() - 1) {
            result += separator;
        }
    }
    return result;
}

}// namespace espresso