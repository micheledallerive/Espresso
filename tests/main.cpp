//
// Created by michele on 08.01.23.
//

#include <gtest/gtest.h>
#include "../src/HTTPMessage.h"
#include "../src/HTTPRequest.h"
#include "../src/HTTPResponse.h"
#include "../src/Middleware.h"

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

TEST(HTTPMessage, testHeadersToString) {
  Espresso::HTTPMessage message("HTTP/1.1", "", "");
  message.setHeader("Connection", "keep-alive");
  message.setHeader("Content-Length", "123");
  message.setHeader("Content-Type", "text/html");
  ASSERT_EQ(message.headersToString(),
            "Connection: keep-alive\r\nContent-Length: 123\r\nContent-Type: text/html\r\n");
}

TEST(HTTPRequest, testConstructor) {
  Espresso::HTTPMethod method = Espresso::HTTPMethod::GET;
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
  ASSERT_EQ(req.getMethod(), Espresso::HTTPMethod::GET);
  ASSERT_EQ(req.getPath(), "/index.html");
  ASSERT_EQ(req.getVersion(), "HTTP/1.1");
  ASSERT_EQ(req.getHeader("Content-Type"), "text/html");
  ASSERT_EQ(req.getHeader("Content-Length"), "123");
  ASSERT_EQ(req.getHeader("Connection"), "keep-alive");
  ASSERT_EQ(req.getBody(), "Hello World!");
}

TEST(Middlewares, testMiddlewareExecute) {
  auto *list = new Espresso::MiddlewareList();
  list->use([](Espresso::HTTPRequest *req,
               Espresso::HTTPResponse *res,
               auto next) {
    res->setHeader("Content-Type", "text/html");
    res->setHeader("Content-Length", "000");
  });
  auto *req = new Espresso::HTTPRequest(Espresso::HTTPMethod::GET,
                                        "/",
                                        "HTTP/1.1",
                                        "",
                                        "");
  auto *res = new Espresso::HTTPResponse();
  list->run(req, res);
  ASSERT_TRUE(res->getHeader("Content-Type") == "text/html");
}

TEST(Middlewares, testMiddlewaresContinue) {
  auto *list = new Espresso::MiddlewareList();
  list->use([](Espresso::HTTPRequest *req,
               Espresso::HTTPResponse *res,
               auto next) {
    res->setHeader("Content-Type", "text/html");
    res->setHeader("Content-Length", "000");
    next();
  });
  list->use([](Espresso::HTTPRequest *req,
               Espresso::HTTPResponse *res,
               auto next) {
    res->setHeader("Content-Length", "123");
  });
  auto *req = new Espresso::HTTPRequest(Espresso::HTTPMethod::GET,
                                        "/",
                                        "HTTP/1.1",
                                        "",
                                        "");
  auto *res = new Espresso::HTTPResponse();
  list->run(req, res);
  ASSERT_EQ(res->getHeader("Content-Type"), "text/html");
  ASSERT_EQ(res->getHeader("Content-Length"), "123");
}

TEST(Middlewares, testMiddlewaresBlocked) {
  auto *list = new Espresso::MiddlewareList();
  list->use([](Espresso::HTTPRequest *req,
               Espresso::HTTPResponse *res,
               auto next) {
    res->setHeader("Content-Type", "text/html");
    res->setHeader("Content-Length", "000");
  });
  list->use([](Espresso::HTTPRequest *req,
               Espresso::HTTPResponse *res,
               auto next) {
    res->setHeader("Content-Length", "123");
  });
  auto *req = new Espresso::HTTPRequest(Espresso::HTTPMethod::GET,
                                        "/",
                                        "HTTP/1.1",
                                        "",
                                        "");
  auto *res = new Espresso::HTTPResponse();
  list->run(req, res);
  ASSERT_EQ(res->getHeader("Content-Type"), "text/html");
  ASSERT_EQ(res->getHeader("Content-Length"), "000");
}
