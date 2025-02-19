#pragma once

#include "custom_data.hpp"
#include "http/headers.hpp"
#include "http/methods.hpp"
#include "routing/path.hpp"
#include <any>
#include <span>
#include <sstream>
#include <string>

namespace espresso::http {
class Request {
public:
    using QueryMap = std::map<std::string, std::string>;
    using Cookies = std::map<std::string, std::string>;

private:
    Method m_method{Method::NONE};
    std::string m_path;
    QueryMap m_query;
    Headers m_headers;
    Cookies m_cookies;
    std::span<char> m_body;
    Path::MatchedGroups m_url_params;

    mutable CustomData m_custom_data;

    void populate_query(std::string_view query);

    void populate_cookies(std::string_view cookies);

public:
    Request() = default;
    ~Request();
    //Request(char* buffer);

    [[nodiscard]] const std::string& path() const;
    [[nodiscard]] const QueryMap& query_params() const;

    [[nodiscard]] Method method() const;

    [[nodiscard]] const Path::MatchedGroups& url_params() const;

    [[nodiscard]] const Headers& headers() const;
    [[nodiscard]] Headers& headers();
    [[nodiscard]] const Cookies& cookies() const;
    [[nodiscard]] Cookies& cookies();

    [[nodiscard]] std::span<char> body() const;

    void set_url_params(Path::MatchedGroups&& mp);

    [[nodiscard]] CustomData& custom_data() const
    {
        return m_custom_data;
    }

    static Request receive_from_network(std::streambuf &stream);

    std::string serialize() const;
};
}// namespace espresso::http