//
// Created by michele on 25.02.23.
//

#include <gtest/gtest.h>
#include "lib/json/JSONObject.h"
#include "lib/json/JSONArray.h"
#include "lib/json/JSONLiteral.h"
#include "lib/json/JSONNumber.h"

using namespace Espresso::JSON;

TEST(JSONObject, Constructor) {
  JSONObject object;
  EXPECT_EQ(object.toJSON(), "{ }");
}

TEST(JSONObject, Add) {
  JSONObject object;
  object["test"] = new JSONLiteral("test");
  EXPECT_EQ(object.toJSON(), "{ \"test\": \"test\" }");
}

TEST(JSONObject, AddNumber) {
  JSONObject object;
  object["test"] = new JSONNumber(1);
  EXPECT_EQ(object.toJSON(), "{ \"test\": 1 }");
}

TEST(JSONObject, AddArray) {
  JSONObject object;
  object["test"] = new JSONArray();
  EXPECT_EQ(object.toJSON(), "{ \"test\": [ ] }");
}

TEST(JSONObject, AddObject) {
  JSONObject object;
  object["test"] = new JSONObject();
  EXPECT_EQ(object.toJSON(), "{ \"test\": { } }");
}

TEST(JSONObject, AddMultiple) {
  JSONObject object;
  object["test"] = new JSONLiteral("test");
  object["test2"] = new JSONLiteral("test2");
  EXPECT_EQ(object.toJSON(), "{ \"test2\": \"test2\", \"test\": \"test\" }");
}

TEST(JSONObject, AddArrayWithValue) {
  JSONObject object;
  JSONArray array;
  array.push_back(new JSONLiteral("test"));
  object["test"] = &array;
  EXPECT_EQ(object.toJSON(), "{ \"test\": [\"test\"] }");
}

TEST(JSONObject, AddObjectWithValue) {
  JSONObject object;
  JSONObject other;
  other["test"] = new JSONLiteral("test");
  object["test"] = &other;
  EXPECT_EQ(object.toJSON(), "{ \"test\": { \"test\": \"test\" } }");
}

TEST(JSONObject, As) {
  auto *obj = new JSONObject();
  EXPECT_EQ(obj->as<JSONObject>(), obj);
  EXPECT_EQ(obj->as<JSONArray>(), nullptr);
}