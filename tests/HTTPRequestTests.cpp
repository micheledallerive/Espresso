//
// Created by michele on 10.01.23.
//

#include <gtest/gtest.h>
#include <HTTPRequest.h>

namespace Espresso {

const std::string base_request =
    "GET /test HTTP/1.1\r\n"
    "Host: localhost:8080\r\n"
    "\r\n"
    "Hello World!";

const std::string rich_request =
    "GET /test?prova=1&prova2=2 HTTP/1.1\r\n"
    "Host: localhost\r\nCookie: prova=1; prova2=2\r\n"
    "\r\n"
    "Hello World!";

TEST(HTTPRequest, Constructor) {
  Espresso::HTTPRequest request(Espresso::HTTPMethod::GET,
                                "/test",
                                "HTTP/1.1",
                                "Host: localhost\r\n",
                                "Hello World!");
  ASSERT_EQ(request.getMethod(), Espresso::HTTPMethod::GET);
  ASSERT_EQ(request.getPath(), "/test");
  ASSERT_EQ(request.getVersion(), "HTTP/1.1");
  ASSERT_EQ(request.getHeader("Host"), "localhost");
  ASSERT_EQ(request.getBody(), "Hello World!");
}

TEST(HTTPRequest, ConstructorFromString) {
  Espresso::HTTPRequest request
      (rich_request);
  ASSERT_EQ(request.getMethod(), Espresso::HTTPMethod::GET);
  ASSERT_EQ(request.getPath(), "/test");
  ASSERT_EQ(request.getVersion(), "HTTP/1.1");
  ASSERT_EQ(request.getHeader("Host"), "localhost");
  ASSERT_TRUE(request.hasHeader("Cookie"));
  ASSERT_EQ(request.getBody(), "Hello World!");
  ASSERT_EQ(request.cookies["prova"], "1");
  ASSERT_EQ(request.cookies["prova2"], "2");
  ASSERT_EQ(request.query["prova"], "1");
  ASSERT_EQ(request.query["prova2"], "2");
}

TEST(HTTPRequest, InvalidCookies) {
  HTTPRequest request(
      "GET / HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Cookie: asdfasdfasdfa\r\n"
        "\r\n"
        ""
      );
  ASSERT_TRUE(request.cookies.empty());
}
}