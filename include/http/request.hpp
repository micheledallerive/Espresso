#pragma once

#include "http/headers.hpp"
#include "http/methods.hpp"
#include "routing/path.hpp"
#include <span>
#include <sstream>
#include <string>

namespace espresso::http {
class Request {
    //private:
public:
    Method m_method{Method::NONE};
    std::string m_path;
    Headers m_headers;
    std::span<char> m_body;
    Path::MatchedGroups m_url_params;

public:
    Request() = default;
    ~Request()
    {
        delete[] m_body.data();
    }
    //Request(char* buffer);

    [[nodiscard]] std::string_view path() const
    {
        return m_path;
    }

    [[nodiscard]] Method method() const
    {
        return m_method;
    }

    [[nodiscard]] const Path::MatchedGroups& url_params() const
    {
        return m_url_params;
    }

    [[nodiscard]] const Headers& headers() const
    {
        return m_headers;
    }

    [[nodiscard]] std::span<char> body() const
    {
        return m_body;
    }

    void set_url_params(Path::MatchedGroups&& mp)
    {
        m_url_params = std::move(mp);
    }

    static Request deserialize(std::span<char> buffer)
    {
        Request req;
        std::stringstream ss(buffer.data());

        {
            std::string method_str;
            ss >> method_str;
            req.m_method = string_to_method(method_str);
        }

        ss >> req.m_path;
        std::string http_version;
        ss >> http_version;// discarded, not needed ;)

        {
            std::string line;
            while (std::getline(ss, line) && !line.empty()) {
                auto pos = line.find(':');
                if (pos != std::string::npos) {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1 + (line[pos + 1] == ' '));
                    req.m_headers.add(key, value);
                }
            }
        }

        char* ptr = new char[buffer.size() - ss.tellg()];
        ss.read(ptr, buffer.size() - ss.tellg());
        req.m_body = std::span<char>(ptr, buffer.size() - ss.tellg());

        return req;
    }
};
}// namespace espresso::http