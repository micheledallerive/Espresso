//
// Created by michele on 27.02.23.
//
#include <gtest/gtest.h>
#include "middleware/FormDataMiddleware.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"

using namespace Espresso;

TEST(FormDataMiddleware, ParseURLEncoded) {
  FormDataMiddleware middleware;
  HTTPRequest request(POST, "/", "HTTP/1.1", "", "");
  request.setHeader("Content-Type", "application/x-www-form-urlencoded");
  request.setRawBody("name=John+Doe&age=42");
  HTTPResponse response;
  middleware.handle(request, response, []() {});
  ASSERT_EQ(request.body.size(), 2);
  ASSERT_EQ(request.body["name"], "John Doe");
  ASSERT_EQ(request.body["age"], "42");

  request.body.clear();
  request.setRawBody("name=John&surname=Doe&full+name=John+Doe");
  middleware.handle(request, response, []() {});
  ASSERT_EQ(request.body.size(), 3);
  ASSERT_EQ(request.body["name"], "John");
  ASSERT_EQ(request.body["surname"], "Doe");
  ASSERT_EQ(request.body["full name"], "John Doe");
}