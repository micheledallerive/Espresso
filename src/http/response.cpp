#include "http/response.hpp"
#include <fstream>

namespace espresso::http {

Response& Response::write(const std::string& str)
{
    m_body.insert(m_body.end(), str.begin(), str.end());
    return *this;
}
Response& Response::status(int status)
{
    m_status = status;
    return *this;
}
Headers& Response::headers()
{
    return m_headers;
}
Response& Response::add_cookie(const Cookie& cookie)
{
    m_headers.insert("Set-Cookie", cookie.serialize());
    return *this;
}
void Response::send_file(const std::filesystem::path& file)
{
    headers().set("Content-Type", mime_type(file));
    //    headers().add("Content-Disposition", "attachment; filename=" + file.filename().string());
    std::ifstream ifs(file, std::ios::binary);
    if (ifs) {
        m_body.insert(m_body.end(), std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
    }
    else {
        status(404);
    }
}
void Response::redirect(const std::string &location)
{
    status(302);
    headers().set("Location", location);
}
std::vector<char> Response::body() const
{
    return m_body;
}
void Response::send_to_network(std::streambuf& stream)
{
    std::ostream ss(&stream);
    ss << "HTTP/1.1 " << m_status << " \r\n";
    for (const auto& [key, value] : m_headers) {
        ss << key << ": " << value << "\r\n";
    }
    ss << "\r\n";
    ss << std::string(m_body.begin(), m_body.end());
    ss.flush();
}
}// namespace espresso::http