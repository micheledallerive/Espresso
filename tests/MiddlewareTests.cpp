//
// Created by michele on 10.01.23.
//
#include <gtest/gtest.h>
#include "Middleware.h"

namespace Espresso {
TEST(MiddlewareList, SimpleUse) {
  MiddlewareList list;
  list.use([](HTTPRequest &request,
              HTTPResponse &response,
              const std::function<void()> &next) {
    std::cout << "executing " << std::endl;
    response.setRawBody("Hello World!");
    next();
  });
  HTTPRequest request(GET, "/", "HTTP/1.1", "", "");
  HTTPResponse response;
  ASSERT_EQ(response.getRawBody(), "");
  list.run(request, response);
  ASSERT_EQ(response.getRawBody(), "Hello World!");
}

TEST(MiddlewareList, ManyMiddlewares) {
  MiddlewareList list;
  list.use([](HTTPRequest &request,
              HTTPResponse &response,
              const std::function<void()> &next) {
    response.setRawBody("Hello World!");
    next();
  });
  list.use([](HTTPRequest &request,
              HTTPResponse &response,
              const std::function<void()> &next) {
    response.setRawBody(response.getRawBody() + " Hello World!");
    next();
  });
  HTTPRequest request(GET, "/", "HTTP/1.1", "", "");
  HTTPResponse response;
  ASSERT_EQ(response.getRawBody(), "");
  list.run(request, response);
  ASSERT_EQ(response.getRawBody(), "Hello World! Hello World!");
}

TEST(MiddlewareList, DontNextMiddleware) {
  MiddlewareList list;
  list.use([](HTTPRequest &request,
              HTTPResponse &response,
              const std::function<void()> &next) {
    response.setRawBody("Hello World!");
  });
  list.use([](HTTPRequest &request,
              HTTPResponse &response,
              const std::function<void()> &next) {
    response.setRawBody(response.getRawBody() + " Hello World!");
  });
  HTTPRequest request(GET, "/", "HTTP/1.1", "", "");
  HTTPResponse response;
  ASSERT_EQ(response.getRawBody(), "");
  list.run(request, response);
  ASSERT_EQ(response.getRawBody(), "Hello World!");
}

TEST(MiddlewareList, UseWithPath) {
  MiddlewareList list;
  list.use("/hello", [](HTTPRequest &request,
                        HTTPResponse &response,
                        const std::function<void()> &next) {
    response.setRawBody("Hello!");
    next();
  });
  list.use("/world", [](HTTPRequest &request,
                        HTTPResponse &response,
                        const std::function<void()> &next) {
    response.setRawBody("World!");
  });
  HTTPRequest request(GET, "/hello", "HTTP/1.1", "", "");
  HTTPResponse response;
  ASSERT_EQ(response.getRawBody(), "");
  list.run(request, response);
  ASSERT_EQ(response.getRawBody(), "Hello!");
}

TEST(MiddlewareList, UseWithPathAndNext) {
  MiddlewareList list;
  list.use("/hello", [](HTTPRequest &request,
                        HTTPResponse &response,
                        const std::function<void()> &next) {
    response.setRawBody("Hello!");
    next();
  });
  list.use("/hello", [](HTTPRequest &request,
                        HTTPResponse &response,
                        const std::function<void()> &next) {
    response.setRawBody(response.getRawBody() + " World!");
    next();
  });
  list.use("/hell", [](HTTPRequest &request,
                       HTTPResponse &response,
                       const std::function<void()> &next) {
    response.setRawBody(response.getRawBody() + " :(");
  });
  HTTPRequest request(GET, "/hello", "HTTP/1.1", "", "");
  HTTPResponse response;
  ASSERT_EQ(response.getRawBody(), "");
  list.run(request, response);
  ASSERT_EQ(response.getRawBody(), "Hello! World!");
}

TEST(MiddlewareList, ClassMiddleware) {
  class SimpleMiddleware : public BaseMiddleware {
   public:
    void handle(HTTPRequest &request,
                HTTPResponse &response,
                const std::function<void()> &next) override {
      std::cout << "Test" << std::endl;
      response.setRawBody("Hello World!");
      next();
    }
  };
  MiddlewareList list;
  SimpleMiddleware m = SimpleMiddleware();
  list.use(m);
  HTTPRequest request(GET, "/", "HTTP/1.1", "", "");
  HTTPResponse response;
  ASSERT_EQ(response.getRawBody(), "");
  list.run(request, response);
  ASSERT_EQ(response.getRawBody(), "Hello World!");
};

}