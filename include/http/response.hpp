#pragma once

#include "cookie.hpp"
#include "headers.hpp"
#include <sstream>
#include <string>
#include <vector>

namespace espresso::http {

class Response {
private:
    int m_status{200};
    Headers m_headers;
    std::vector<char> m_body;

public:
    Response() = default;

    Response& write(const std::string& str);

    Response& status(int status);

    Headers& headers();

    Response& add_cookie(const Cookie& cookie);

    std::string serialize();
};
}// namespace espresso::http