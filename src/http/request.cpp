#include "http/request.hpp"

namespace espresso::http {

Request::~Request()
{
    delete[] m_body.data();
}
const std::string &Request::path() const
{
    return m_path;
}
Method Request::method() const
{
    return m_method;
}
const Path::MatchedGroups& Request::url_params() const
{
    return m_url_params;
}
const Headers& Request::headers() const
{
    return m_headers;
}
std::span<char> Request::body() const
{
    return m_body;
}
void Request::set_url_params(Path::MatchedGroups&& mp)
{
    m_url_params = std::move(mp);
}
Request Request::deserialize(std::span<char> buffer)
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
}// namespace espresso