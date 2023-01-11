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

TEST(HTTPRequest, PartialRequest) {
  HTTPRequest request("GET /test HTTP/1.1\r\n");
  ASSERT_EQ(request.getMethod(), GET);
  ASSERT_EQ(request.getPath(), "/test");
  ASSERT_EQ(request.getVersion(), "HTTP/1.1");
  ASSERT_EQ(request.headersToString(), "");
  ASSERT_EQ(request.getBody(), "");
}

TEST(HTTPRequest, WithoutBody) {
  HTTPRequest request("GET / HTTP/1.1\r\n"
                      "Host: localhost\r\n"
                      "\r\n");
  ASSERT_EQ(request.getMethod(), GET);
  ASSERT_TRUE(request.getBody().empty());
}

TEST(HTTPRequest, WithoutHeaders) {
  HTTPRequest request("GET / HTTP/1.1\r\n"
                      "\r\n"
                      "Hello World!");
  ASSERT_EQ(request.getMethod(), GET);
  ASSERT_EQ(request.getBody(), "Hello World!");
}

TEST(HTTPRequest, InvalidHeaders) {
  HTTPRequest request("GET / HTTP/1.1\r\n"
                      "Accept:\r\n"
                      "Invalid\r\n"
                      ": test\r\n"
                      "A: B\r\n"
                      "\r\n"
                      "");
  ASSERT_FALSE(request.hasHeader("Accept"));
  ASSERT_FALSE(request.hasHeader("Invalid"));
  ASSERT_FALSE(request.hasHeader(""));
  ASSERT_TRUE(request.hasHeader("A"));
  ASSERT_EQ(request.getHeader("A"), "B");
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
  request = HTTPRequest(
      "GET / HTTP/1.1\r\n"
      "Host: localhost\r\n"
      "Cookie: ;;;;;;;;\r\n"
      "\r\n"
      ""
  );
  ASSERT_TRUE(request.cookies.empty());
}

TEST(HTTPRequest, EmptyValueCookies) {
  HTTPRequest request(
      "GET / HTTP/1.1\r\n"
      "Host: localhost\r\n"
      "Cookie: t=; test=a; =c\r\n"
      "\r\n"
      ""
  );
  ASSERT_FALSE(request.cookies.empty());
  ASSERT_EQ(request.cookies["t"], "");
  ASSERT_EQ(request.cookies["test"], "a");
  ASSERT_EQ(request.cookies.size(), 2);
}

TEST(HTTPRequest, NoCookies) {
  HTTPRequest request(base_request);
  ASSERT_EQ(request.cookies.size(), 0);
  ASSERT_ANY_THROW(request.cookies.at("test"));
}

TEST(HTTPRequest, NoQuery) {
  HTTPRequest request(base_request);
  ASSERT_EQ(request.query.size(), 0);
  ASSERT_ANY_THROW(request.query.at("test"));
}

TEST(HTTPRequest, InvalidQuery) {
  HTTPRequest request(
      "GET /?=a&t=&c=b HTTP/1.1\r\n"
      "\r\n"
      ""
  );
  ASSERT_EQ(request.query.size(), 2);
  ASSERT_EQ(request.query["t"], "");
  ASSERT_EQ(request.query["c"], "b");
}

}