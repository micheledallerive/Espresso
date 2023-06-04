//
// Created by michele on 25.02.23.
//

#include <gtest/gtest.h>
#include "lib/json/json_array.h"
#include "lib/json/json_literal.h"
#include "lib/json/json_number.h"
#include "lib/json/json_boolean.h"

using namespace Espresso::JSON;

TEST(JSONArray, Constructor) {
  JSONArray array;
  EXPECT_EQ(array.toJSON(), "[ ]");
}

TEST(JSONArray, Add) {
  JSONArray array;
  array.push_back(std::make_shared<JSONLiteral>("test"));
  EXPECT_EQ(array.toJSON(), "[\"test\"]");
}

TEST(JSONArray, AddMultiple) {
  JSONArray array;
  array.push_back(std::make_shared<JSONLiteral>("test"));
  array.push_back(std::make_shared<JSONLiteral>("test2"));
  EXPECT_EQ(array.toJSON(), "[\"test\", \"test2\"]");
}

TEST(JSONArray, AddMultipleWithDelete) {
  JSONArray array;
  array.push_back(std::make_shared<JSONLiteral>("test"));
  array.push_back(std::make_shared<JSONLiteral>("test2"));
  array.erase(array.begin());
  EXPECT_EQ(array.toJSON(), "[\"test2\"]");
}

TEST(JSONArray, AddMultipleTypes) {
  JSONArray array;
  array.push_back(std::make_shared<JSONLiteral>("test"));
  array.push_back(std::make_shared<JSONBoolean>(false));
  array.push_back(std::make_shared<JSONNumber>(1.15));
  EXPECT_EQ(array.toJSON(), "[\"test\", false, 1.15]");
}

TEST(JSONArray, NullElement) {
  JSONArray array;
  array.push_back(std::make_shared<JSONLiteral>("test"));
  array.push_back(nullptr);
  array.push_back(std::make_shared<JSONLiteral>("test2"));
  EXPECT_EQ(array.toJSON(), "[\"test\", null, \"test2\"]");
}

TEST(JSONArray, As) {
  auto arr = std::make_shared<JSONArray>();
  EXPECT_TRUE(arr->as<JSONArray>() != nullptr);
  EXPECT_TRUE(arr->as<JSONLiteral>() == nullptr);
}


