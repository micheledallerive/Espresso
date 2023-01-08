//
// Created by michele on 08.01.23.
//

#include <gtest/gtest.h>
#include "../src/HTTPMessage.h"
#include "../src/HTTPRequest.h"

TEST(HTTPMessage, testParseHeaders) {
  std::string headers = "Content-Type: text/html\r\n"
                        "Content-Length: 123\r\n"
                        "Connection: keep-alive\r\n";
  Espresso::HTTPMessage message("HTTP/1.1", headers, "");
  ASSERT_EQ(message.getHeader("Content-Type"), "text/html");
}

TEST(HTTPMessage, testSetHeader) {
  Espresso::HTTPMessage message("HTTP/1.1", "", "");
  message.setHeader("Content-Type", "text/html");
  ASSERT_EQ(message.getHeader("Content-Type"), "text/html");
}

TEST(HTTPRequest, testConstructor) {
  std::string method = "GET";
  std::string path = "/index.html";
  std::string version = "HTTP/1.1";
  std::string headers = "Content-Type: text/html\r\n"
                        "Content-Length: 123\r\n"
                        "Connection: keep-alive\r\n";
  std::string body = "Hello World!";
  Espresso::HTTPRequest request(method, path, version, headers, body);
  ASSERT_EQ(request.getMethod(), method);
  ASSERT_EQ(request.getPath(), path);
  ASSERT_EQ(request.getVersion(), version);
  ASSERT_EQ(request.getHeader("Content-Type"), "text/html");
  ASSERT_EQ(request.getHeader("Content-Length"), "123");
  ASSERT_EQ(request.getHeader("Connection"), "keep-alive");
  ASSERT_EQ(request.getBody(), body);
}

TEST(HTTPRequest, testConstructorFromRequest) {
    std::string request = "GET /index.html HTTP/1.1\r\n"
                            "Content-Type: text/html\r\n"
                            "Content-Length: 123\r\n"
                            "Connection: keep-alive\r\n"
                            "\r\n"
                            "Hello World!";
    Espresso::HTTPRequest req(request);
    ASSERT_EQ(req.getMethod(), "GET");
    ASSERT_EQ(req.getPath(), "/index.html");
    ASSERT_EQ(req.getVersion(), "HTTP/1.1");
    ASSERT_EQ(req.getHeader("Content-Type"), "text/html");
    ASSERT_EQ(req.getHeader("Content-Length"), "123");
    ASSERT_EQ(req.getHeader("Connection"), "keep-alive");
    ASSERT_EQ(req.getBody(), "Hello World!");
}