//
// Created by michele on 08.01.23.
//

#include "requests/http_response.h"
#include "server.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace Espresso {

std::unordered_map<int, std::string> HTTP_REASONS = {
    {100, "Continue"},
    {101, "Switching Protocols"},
    {102, "Processing"},
    {103, "Early Hints"},
    {200, "OK"},
    {201, "Created"},
    {202, "Accepted"},
    {203, "Non-Authoritative Information"},
    {204, "No Content"},
    {205, "Reset Content"},
    {206, "Partial Content"},
    {207, "Multi-Status"},
    {208, "Already Reported"},
    {226, "IM Used"},
    {300, "Multiple Choices"},
    {301, "Moved Permanently"},
    {302, "Found"},
    {303, "See Other"},
    {304, "Not Modified"},
    {305, "Use Proxy"},
    {306, "Switch Proxy"},
    {307, "Temporary Redirect"},
    {308, "Permanent Redirect"},
    {400, "Bad Request"},
    {401, "Unauthorized"},
    {402, "Payment Required"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {405, "Method Not Allowed"},
    {406, "Not Acceptable"},
    {407, "Proxy Authentication Required"},
    {408, "Request Timeout"},
    {409, "Conflict"},
    {410, "Gone"},
    {411, "Length Required"},
    {412, "Precondition Failed"},
    {413, "Payload Too Large"},
    {414, "URI Too Long"},
    {415, "Unsupported Media Type"},
    {416, "Range Not Satisfiable"},
    {417, "Expectation Failed"},
    {418, "I'm a teapot"},
    {421, "Misdirected Request"},
    {422, "Unprocessable Entity"},
    {423, "Locked"},
    {424, "Failed Dependency"},
    {425, "Too Early"},
    {426, "Upgrade Required"},
    {428, "Precondition Required"},
    {429, "Too Many Requests"},
    {431, "Request Header Fields Too Large"},
    {451, "Unavailable For Legal Reasons"},
    {500, "Internal Server Error"},
    {501, "Not Implemented"},
    {502, "Bad Gateway"},
    {503, "Service Unavailable"},
    {504, "Gateway Timeout"},
    {505, "HTTP Version Not Supported"},
    {506, "Variant Also Negotiates"},
    {507, "Insufficient Storage"},
    {508, "Loop Detected"},
    {510, "Not Extended"},
    {511, "Network Authentication Required"}};

HTTPResponse::HTTPResponse() : HTTPMessage() {
  this->status_ = 200;
  this->version_ = "HTTP/1.1";
  this->addDefaultHeaders_();
}

HTTPResponse::~HTTPResponse() = default;

void HTTPResponse::addDefaultHeaders_() {
  this->setHeader("Date", getUTCDate());
}

HTTPResponse &HTTPResponse::setStatus(int status) {
  if (HTTP_REASONS.find(status) == HTTP_REASONS.end()) {
    throw std::invalid_argument("Invalid status code");
  }
  this->status_ = status;
  return *this;
}

int HTTPResponse::getStatus() const { return this->status_; }

std::string HTTPResponse::toString() {
  std::string response = this->getVersion() + " " + std::to_string(this->status_) + " " + HTTP_REASONS[this->status_] + "\r\n";
  response += this->headersToString();
  for (const auto &it : this->cookies_) {
    response += "Set-Cookie: " + it.second.toString() + "\r\n";
  }
  response += "\r\n";
  response += this->getBody();
  return response;
}

void HTTPResponse::send(const std::string &body) {
  this->setBody(body);
  this->setHeader("Content-Length", std::to_string(body.length()));
}

void HTTPResponse::sendFile(const std::string &path) {
  std::string final_path = getAbsolutePath(path);
  std::ifstream file(final_path);
  if (!file.fail() && file.is_open()) {
    std::stringstream buffer;
    buffer << file.rdbuf();
    this->setBody(buffer.str());
    this->setHeader("Content-Length", std::to_string(buffer.str().length()));
    this->setStatus(200);
  } else {
    throw std::runtime_error("File not found");
  }
}

void HTTPResponse::downloadFile(const std::string &path,
                                const std::string &filename) {
  this->sendFile(path);
  std::string disposition =
      "attachment" + (filename.empty() ? "" : "; filename=\"" + filename + "\"");
  this->setHeader("Content-Disposition", disposition);
}

void HTTPResponse::redirect(const std::string &path, int status) {
  this->setHeader("Location", path);
  this->setStatus(status);
}
void HTTPResponse::setCookie(const Cookie &cookie) {
  this->cookies_[cookie.name] = cookie;
}

void HTTPResponse::setCookie(const std::string &name,
                             const std::string &value) {
  this->setCookie(Cookie(name, value));
}

void HTTPResponse::deleteCookie(const std::string &name) {
  this->setCookie(Cookie(name, "", "", "", 0, false, false));
}

}// namespace Espresso