#pragma once

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

namespace espresso {

/**
 * Return -1 if s1 < s2, 0 if s1 == s2, 1 if s1 > s2.
 * All comparisons are case-insensitive.
 */
static int compare_case_insensitive(std::string_view s1, std::string_view s2)
{
    size_t cmp_size = std::min(s1.length(), s2.length());
    for (size_t i = 0; i < cmp_size; ++i) {
        char c1 = static_cast<char>(std::tolower(s1[i]));
        char c2 = static_cast<char>(std::tolower(s2[i]));
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
    }
    if (s1.length() < s2.length()) return -1;
    if (s1.length() > s2.length()) return 1;
    return 0;
}

struct case_insensitive_comparator {
    bool operator()(std::string_view s1, std::string_view s2) const
    {
        return compare_case_insensitive(s1, s2) < 0;
    }
};

[[maybe_unused]] static bool equals_case_insensitive(std::string_view s1, std::string_view s2)
{
    if (s1.size() != s2.size()) return false;
    return compare_case_insensitive(s1, s2) == 0;
}

[[maybe_unused]] static std::string concatenate(const std::vector<std::string>& strings, const std::string& separator)
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