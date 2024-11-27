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
    m_headers.add("Set-Cookie", cookie.serialize());
    return *this;
}
std::string Response::serialize()
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
void Response::send_file(const std::filesystem::path& file)
{
    headers().add("Content-Type", mime_type(file));
    //    headers().add("Content-Disposition", "attachment; filename=" + file.filename().string());
    std::ifstream ifs(file, std::ios::binary);
    if (ifs) {
        m_body.insert(m_body.end(), std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
    }
    else {
        status(404);
    }
}
const std::vector<char> Response::body() const
{
    return m_body;
}
}// namespace espresso::http