#pragma once

#include "http/headers.hpp"
#include "http/methods.hpp"
#include "routing/path.hpp"
#include <span>
#include <sstream>
#include <string>

namespace espresso::http {
class Request {
public:
    using QueryMap = std::map<std::string,std::string>;
private:
    Method m_method{Method::NONE};
    std::string m_path;
    QueryMap m_query;
    Headers m_headers;
    std::span<char> m_body;
    Path::MatchedGroups m_url_params;

    void populate_query(std::string_view query);

public:
    Request() = default;
    ~Request();
    //Request(char* buffer);

    [[nodiscard]] const std::string& path() const;
    [[nodiscard]] const QueryMap &query_params() const;

    [[nodiscard]] Method method() const;

    [[nodiscard]] const Path::MatchedGroups& url_params() const;

    [[nodiscard]] const Headers& headers() const;

    [[nodiscard]] std::span<char> body() const;

    void set_url_params(Path::MatchedGroups&& mp);

    static Request deserialize(std::span<char> buffer);
};
}// namespace espresso::http