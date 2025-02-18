#include "http/request.hpp"
#include "utils/string.hpp"
#include "utils/network_stream.hpp"

namespace espresso::http {

Request::~Request()
{
    delete[] m_body.data();
}
const std::string& Request::path() const
{
    return m_path;
}
const Request::QueryMap& Request::query_params() const
{
    return m_query;
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
Headers& Request::headers()
{
    return m_headers;
}
const Request::Cookies& Request::cookies() const
{
    return m_cookies;
}
Request::Cookies& Request::cookies()
{
    return m_cookies;
}
std::span<char> Request::body() const
{
    return m_body;
}
void Request::set_url_params(Path::MatchedGroups&& mp)
{
    m_url_params = std::move(mp);
}

void Request::populate_query(std::string_view query)
{
    std::stringstream ss(query.data());
    std::string key;
    std::string value;
    while (std::getline(ss, key, '=') && std::getline(ss, value, '&')) {
        m_query[key] = value;
    }
}

void Request::populate_cookies(std::string_view cookies)
{
    std::stringstream ss(cookies.data());
    std::string key;
    std::string value;
    while (std::getline(ss, key, '=') && std::getline(ss, value, ';')) {
        m_cookies[key] = value;
    }
}

Request Request::receive_from_network(std::streambuf &stream)
{
    Request req;
    std::istream ss(&stream);

    {
        std::string method_str;
        ss >> method_str;
        req.m_method = string_to_method(method_str);
    }

    ss >> req.m_path;

    {
        auto pos = req.m_path.find('?');
        if (pos != std::string::npos) {
            req.populate_query(req.m_path.substr(pos + 1));
            req.m_path = req.m_path.substr(0, pos);
        }
    }

    std::string http_version;
    ss >> http_version;// discarded, not needed ;)

    // right after http_version, there is \r\n, which we want to discard as well
    ss.ignore(2);
    {
        std::string line;
        while (std::getline(ss, line)
               && !line.empty()
               && line[0] != '\r') {
            // Apparently if the body is empty, Firefox does not send the empty line but terminates the request with the last header, making the ending \r absent
            //assert(line.back() == '\r');
            if (line.back() == '\r') {
                line.pop_back();
            }
            auto pos = line.find(':');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1 + (line[pos + 1] == ' '));
                if (equals_case_insensitive(key, "Cookie")) {
                    req.populate_cookies(value);
                }
                else {
                    req.m_headers.insert(key, value);
                }
            }
        }
    }

    bool has_content_length = req.m_headers.contains("Content-Length");
    std::size_t body_len = has_content_length ? std::stoi(req.headers()["Content-Length"]) : 0;
    char* ptr = new char[body_len];
    ss.read(ptr, body_len);
    req.m_body = std::span<char>(ptr, body_len);

    return req;
}
}// namespace espresso::http