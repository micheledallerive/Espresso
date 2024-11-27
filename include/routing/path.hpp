#pragma once

#include <cassert>
#include <iostream>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace espresso {

class Path {
public:
    using MatchedGroups = std::map<std::string, std::string>;

private:
    std::string m_path;
    std::vector<std::string_view> m_parts;
    std::vector<uint8_t> m_special;

    static bool valid_char(char c) noexcept;
    static std::vector<std::string_view> split_url(std::string_view str);
    static std::vector<std::string_view> split_url(const std::string& str);

public:
    explicit Path(std::string_view path);

    [[nodiscard]] std::optional<MatchedGroups> matches(const std::vector<std::string_view>& url_parts) const;
    [[nodiscard]] std::optional<MatchedGroups> matches(std::string_view url) const;

    [[nodiscard]] const std::string &path() const;
};

}// namespace espresso