#pragma once

#include "cookie.hpp"
#include "http/headers.hpp"
#include "http/mime.hpp"
#include <filesystem>
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

    void send_file(const std::filesystem::path& file);

    Response& status(int status);

    Headers& headers();

    Response& add_cookie(const Cookie& cookie);

    std::string serialize();
};
}// namespace espresso::http