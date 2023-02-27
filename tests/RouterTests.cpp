//
// Created by michele on 10.01.23.
//

#include <gtest/gtest.h>

#include <utility>
#include "Router.h"

namespace Espresso {

HTTPRequest generateReq(HTTPMethod method, std::string path) {
  return {method, std::move(path), "HTTP/1.1", "", ""};
}

TEST(Router, Get) {
  Router router;
  router.get("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send(request.getRawBody() + "Hello World!");
  });
  router.post("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send(request.getRawBody() + "Hello World!");
  });
  HTTPRequest request = generateReq(GET, "/test");
  HTTPResponse response;
  ASSERT_EQ(response.getRawBody(), "");
  router.executeMatchingRoute(request, response);
  ASSERT_EQ(response.getRawBody(), "Hello World!");
}

TEST(Router, Post) {
  Router router;
  router.post("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send("Hello World!");
  });
  router.get("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send(request.getRawBody() + "Hello World!");
  });
  HTTPRequest request = generateReq(POST, "/test");
  HTTPResponse response;
  ASSERT_EQ(response.getRawBody(), "");
  router.executeMatchingRoute(request, response);
  ASSERT_EQ(response.getRawBody(), "Hello World!");
}

TEST(Router, Put) {
  Router router;
  router.put("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send("Hello World!");
  });
  router.get("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send(request.getRawBody() + "Hello World!");
  });
  HTTPRequest request = generateReq(PUT, "/test");
  HTTPResponse response;
  ASSERT_EQ(response.getRawBody(), "");
  router.executeMatchingRoute(request, response);
  ASSERT_EQ(response.getRawBody(), "Hello World!");
}

TEST(Router, Delete) {
  Router router;
  router.del("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send("Hello World!");
  });
  router.get("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send(request.getRawBody() + "Hello World!");
  });
  HTTPRequest request = generateReq(DELETE, "/test");
  HTTPResponse response;
  ASSERT_EQ(response.getRawBody(), "");
  router.executeMatchingRoute(request, response);
  ASSERT_EQ(response.getRawBody(), "Hello World!");
}

TEST(Router, Patch) {
  Router router;
  router.patch("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send("Hello World!");
  });
  router.get("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send(request.getRawBody() + "Hello World!");
  });
  HTTPRequest request = generateReq(PATCH, "/test");
  HTTPResponse response;
  ASSERT_EQ(response.getRawBody(), "");
  router.executeMatchingRoute(request, response);
  ASSERT_EQ(response.getRawBody(), "Hello World!");
}

TEST(Router, Options) {
  Router router;
  router.options("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send("Hello World!");
  });
  router.get("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send(request.getRawBody() + "Hello World!");
  });
  HTTPRequest request = generateReq(OPTIONS, "/test");
  HTTPResponse response;
  ASSERT_EQ(response.getRawBody(), "");
  router.executeMatchingRoute(request, response);
  ASSERT_EQ(response.getRawBody(), "Hello World!");
}

TEST(Router, Head) {
  Router router;
  router.head("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send("Hello World!");
  });
  router.get("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send(request.getRawBody() + "Hello World!");
  });
  HTTPRequest request = generateReq(HEAD, "/test");
  HTTPResponse response;
  ASSERT_EQ(response.getRawBody(), "");
  router.executeMatchingRoute(request, response);
  ASSERT_EQ(response.getRawBody(), "Hello World!");
}

TEST(Router, Trace) {
  Router router;
  router.trace("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send("Hello World!");
  });
  router.get("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send(request.getRawBody() + "Hello World!");
  });
  HTTPRequest request = generateReq(TRACE, "/test");
  HTTPResponse response;
  ASSERT_EQ(response.getRawBody(), "");
  router.executeMatchingRoute(request, response);
  ASSERT_EQ(response.getRawBody(), "Hello World!");
}

TEST(Router, Connect) {
  Router router;
  router.connect("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send("Hello World!");
  });
  router.get("/test", [](HTTPRequest &request, HTTPResponse &response) {
    response.send(request.getRawBody() + "Hello World!");
  });
  HTTPRequest request = generateReq(CONNECT, "/test");
  HTTPResponse response;
  ASSERT_EQ(response.getRawBody(), "");
  router.executeMatchingRoute(request, response);
  ASSERT_EQ(response.getRawBody(), "Hello World!");
}

TEST(Router, MultipleRoutes) {
  Router router;
  router.addRoute((std::vector<std::string>) {"/test", "/hello"},
                  GET,
                  [](HTTPRequest &request, HTTPResponse &response) {
                    response.send("Hello World!");
                  });
  HTTPRequest request1 = generateReq(GET, "/test");
  HTTPRequest request2 = generateReq(GET, "/hello");
  HTTPRequest request3 = generateReq(GET, "/world");

  HTTPResponse response;

  router.executeMatchingRoute(request1, response);
  ASSERT_EQ(response.getRawBody(), "Hello World!");

  response = HTTPResponse();
  router.executeMatchingRoute(request2, response);
  ASSERT_EQ(response.getRawBody(), "Hello World!");

  response = HTTPResponse();
  router.executeMatchingRoute(request3, response);
  ASSERT_EQ(response.getRawBody(), "");
}

TEST(Router, MultiHandlers) {
  Router router;
  router.route("/test", {
      {GET, [](HTTPRequest &request, HTTPResponse &response) {
        response.send("Hello World!");
      }},
      {POST, [](HTTPRequest &request, HTTPResponse &response) {
        response.send("Hello World!");
      }},
  });
  HTTPRequest request1 = generateReq(GET, "/test");
  HTTPRequest request2 = generateReq(POST, "/test");
  HTTPRequest request3 = generateReq(PUT, "/test");

  HTTPResponse response;
  router.executeMatchingRoute(request1, response);
  ASSERT_EQ(response.getRawBody(), "Hello World!");

  response = HTTPResponse();
  router.executeMatchingRoute(request2, response);
  ASSERT_EQ(response.getRawBody(), "Hello World!");

  response = HTTPResponse();
  router.executeMatchingRoute(request3, response);
  ASSERT_EQ(response.getRawBody(), "");

}

TEST(Router, UrlParams) {
  Router router;
  router.get("/test/:id", [](HTTPRequest &request, HTTPResponse &response) {
    response.setStatus(400);
    response.send(request.params["id"]);
  });
  HTTPRequest request = generateReq(GET, "/test/123");
  HTTPResponse response;
  router.executeMatchingRoute(request, response);
  ASSERT_EQ(response.getStatus(), 400);
  ASSERT_EQ(response.getRawBody(), "123");
  ASSERT_FALSE(request.params.empty());
  ASSERT_EQ(request.params.size(), 1);
}

}