#pragma once

#include <string>
#include <string_view>
#include <algorithm>

namespace espresso {

static bool equals_case_insensitive(std::string_view s1, std::string_view s2)
{
    return s1.size() == s2.size() && std::equal(s1.begin(), s1.end(), s2.begin(), s2.end(), [](char a, char b) {
               return std::tolower(a) == std::tolower(b);
           });
}

}// namespace espresso