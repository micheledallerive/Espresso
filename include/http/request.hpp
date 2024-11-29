#pragma once

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

    mutable std::map<std::string, std::any> m_custom_data;

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
    [[nodiscard]] const Cookies& cookies() const;

    [[nodiscard]] std::span<char> body() const;

    void set_url_params(Path::MatchedGroups&& mp);

    [[nodiscard]] const std::map<std::string, std::any>& custom_data() const;
    [[nodiscard]] const std::any &custom_data(std::string_view s) const;
    template<typename T>
    [[nodiscard]] const T& custom_data_as(std::string_view s) const
    {
        return std::any_cast<const T&>(m_custom_data.at(s.data()));
    }
    template<typename T>
    const Request& set_data(const std::string& key, T value) const
    {
        m_custom_data[key] = std::move(value);
        return *this;
    }

    static Request deserialize(std::span<char> buffer);
};
}// namespace espresso::http