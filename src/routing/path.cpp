#include "routing/path.hpp"
#include <cassert>

namespace espresso {

bool Path::valid_char(char c) noexcept
{
    // RFC 3986 unreserved characters
    return isalnum(c) || c == '_' || c == '-' || c == '~' || c == '.';
}
Path::Path(std::string_view path) : m_path(path),
                                    m_parts(split_url(m_path)),
                                    m_special(m_parts.size(), false)
{
    for (size_t i = 0; i < m_parts.size(); ++i) {
        if ((m_parts[i].find('*') != std::string_view::npos)
            || (m_parts[i].find('?') != std::string_view::npos)) {
            m_special[i] = 1;
        }
        else if (m_parts[i].size() > 2 && m_parts[i][0] == '{' && m_parts[i].back() == '}') {
            for (size_t j = 1; j < m_parts[i].size() - 1; ++j) {
                if (!isalpha(m_parts[i][j])) throw std::runtime_error("Named group can only be named with alphabetical characters");
            }
            m_special[i] = 2;
        }

        if (!m_special[i]) {
            for (char c : m_parts[i]) {
                if (!valid_char(c)) {
                    throw std::runtime_error("Invalid character in path");
                }
            }
        }
    }
}
std::optional<Path::MatchedGroups> Path::matches(const std::vector<std::string_view>& url_parts) const
{
    if (m_parts.size() != url_parts.size()) {
        return {};
    }
    MatchedGroups matched_groups;
    for (size_t i = 0; i < m_parts.size(); ++i) {
        assert(m_special[i] < 3);

        if (m_special[i] == 0) {
            if (m_parts[i] != url_parts[i]) {
                return {};
            }
        }
        else if (m_special[i] == 1) {
            size_t char_idx;
            for (char_idx = 0; char_idx < m_parts[i].size(); ++char_idx) {
                if (m_parts[i][char_idx] == '*') {
                    goto next;
                }
                if (m_parts[i][char_idx] == '?') {
                    continue;
                }
                if (char_idx >= url_parts[i].size())
                    return {};
                if (m_parts[i][char_idx] != url_parts[i][char_idx]) {
                    return {};
                }
            }
            if (char_idx != url_parts[i].size()) {
                return {};
            }
        }
        else if (m_special[i] == 2) {
            matched_groups[std::string(m_parts[i].substr(1, m_parts[i].size() - 2))] = std::string(url_parts[i]);
        }
    next:
    }
    return matched_groups;
}
std::optional<Path::MatchedGroups> Path::matches(std::string_view url) const
{
    return matches(split_url(url));
}
std::vector<std::string_view> Path::split_url(std::string_view str)
{
    std::vector<std::string_view> res;
    std::string_view p = str;
    if (p[0] == '/') p.remove_prefix(1);
    while (!p.empty()) {
        auto pos = p.find('/');
        if (pos == std::string_view::npos) {
            res.push_back(p);
            break;
        }
        res.push_back(p.substr(0, pos));
        p.remove_prefix(pos + 1);
    }
    return res;
}
std::vector<std::string> Path::split_url(const std::string& str)
{
    std::vector<std::string> res;
    const auto& res_view = split_url(std::string_view(str));
    for (const auto& view : res_view) {
        res.emplace_back(view);
    }
    return res;
}
const std::string& Path::path() const
{
    return m_path;
}
}// namespace espresso