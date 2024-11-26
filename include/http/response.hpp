#pragma once

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

    Response &write(const std::string& str)
    {
        m_body.insert(m_body.end(), str.begin(), str.end());
        return *this;
    }

    Response &status(int status) {
        m_status = status;
        return *this;
    }

    Headers &headers() {
        return m_headers;
    }

    std::string serialize()
    {
        std::string res;
        std::ostringstream ss;
        ss << "HTTP/1.1 " << m_status << " \r\n";
        for (const auto& [key, value] : m_headers) {
            ss << key << ": " << value << "\r\n";
        }
        ss << "\r\n";
        ss << std::string(m_body.begin(), m_body.end());
        return ss.str();
    }
};
}// namespace espresso::http