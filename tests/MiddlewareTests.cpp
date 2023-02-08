//
// Created by michele on 10.01.23.
//
#include <gtest/gtest.h>
#include "Middleware.h"

using namespace Espresso;
TEST(MiddlewareList, SimpleUse) {
  MiddlewareList list;
  list.use([](HTTPRequest &request,
              HTTPResponse &response,
              const std::function<void()> &next) {
    std::cout << "executing " << std::endl;
    response.setBody("Hello World!");
    next();
  });
  HTTPRequest request(GET, "/", "HTTP/1.1", "", "");
  HTTPResponse response;
  ASSERT_EQ(response.getBody(), "");
  list.run(request, response);
  ASSERT_EQ(response.getBody(), "Hello World!");
}

TEST(MiddlewareList, ManyMiddlewares) {
  MiddlewareList list;
  list.use([](HTTPRequest &request,
              HTTPResponse &response,
              const std::function<void()> &next) {
    response.setBody("Hello World!");
    next();
  });
  list.use([](HTTPRequest &request,
              HTTPResponse &response,
              const std::function<void()> &next) {
    response.setBody(response.getBody() + " Hello World!");
    next();
  });
  HTTPRequest request(GET, "/", "HTTP/1.1", "", "");
  HTTPResponse response;
  ASSERT_EQ(response.getBody(), "");
  list.run(request, response);
  ASSERT_EQ(response.getBody(), "Hello World! Hello World!");
}

TEST(MiddlewareList, DontNextMiddleware) {
  MiddlewareList list;
  list.use([](HTTPRequest &request,
              HTTPResponse &response,
              const std::function<void()> &next) {
    response.setBody("Hello World!");
  });
  list.use([](HTTPRequest &request,
              HTTPResponse &response,
              const std::function<void()> &next) {
    response.setBody(response.getBody() + " Hello World!");
  });
  HTTPRequest request(GET, "/", "HTTP/1.1", "", "");
  HTTPResponse response;
  ASSERT_EQ(response.getBody(), "");
  list.run(request, response);
  ASSERT_EQ(response.getBody(), "Hello World!");
}

TEST(MiddlewareList, UseWithPath) {
  MiddlewareList list;
  list.use("/hello", [](HTTPRequest &request,
                        HTTPResponse &response,
                        const std::function<void()> &next) {
    response.setBody("Hello!");
    next();
  });
  list.use("/world", [](HTTPRequest &request,
                        HTTPResponse &response,
                        const std::function<void()> &next) {
    response.setBody("World!");
  });
  HTTPRequest request(GET, "/hello", "HTTP/1.1", "", "");
  HTTPResponse response;
  ASSERT_EQ(response.getBody(), "");
  list.run(request, response);
  ASSERT_EQ(response.getBody(), "Hello!");
}

TEST(MiddlewareList, UseWithPathAndNext) {
  MiddlewareList list;
  list.use("/hello", [](HTTPRequest &request,
                        HTTPResponse &response,
                        const std::function<void()> &next) {
    response.setBody("Hello!");
    next();
  });
  list.use("/hello", [](HTTPRequest &request,
                        HTTPResponse &response,
                        const std::function<void()> &next) {
    response.setBody(response.getBody() + " World!");
    next();
  });
  list.use("/hell", [](HTTPRequest &request,
                        HTTPResponse &response,
                        const std::function<void()> &next) {
    response.setBody(response.getBody() + " :(");
  });
  HTTPRequest request(GET, "/hello", "HTTP/1.1", "", "");
  HTTPResponse response;
  ASSERT_EQ(response.getBody(), "");
  list.run(request, response);
  ASSERT_EQ(response.getBody(), "Hello! World!");
}