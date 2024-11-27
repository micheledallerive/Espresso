#pragma once

#include "http/headers.hpp"
#include "http/methods.hpp"
#include "routing/path.hpp"
#include <span>
#include <sstream>
#include <string>

namespace espresso::http {
class Request {
private:
    Method m_method{Method::NONE};
    std::string m_path;
    Headers m_headers;
    std::span<char> m_body;
    Path::MatchedGroups m_url_params;

public:
    Request() = default;
    ~Request();
    //Request(char* buffer);

    [[nodiscard]] const std::string& path() const;

    [[nodiscard]] Method method() const;

    [[nodiscard]] const Path::MatchedGroups& url_params() const;

    [[nodiscard]] const Headers& headers() const;

    [[nodiscard]] std::span<char> body() const;

    void set_url_params(Path::MatchedGroups&& mp);

    static Request deserialize(std::span<char> buffer);
};
}// namespace espresso::http