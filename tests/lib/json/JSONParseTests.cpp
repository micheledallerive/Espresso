//
// Created by michele on 25.02.23.
//

#include <gtest/gtest.h>
#include "lib/json/json.h"
#include "lib/json/JSONLiteral.h"
#include "lib/json/JSONArray.h"
#include "lib/json/JSONObject.h"
#include "lib/json/JSONNumber.h"
#include "lib/json/JSONBoolean.h"

using namespace Espresso::JSON;

TEST(JSONParse, EmptyObject) {
  JSONBase *json = parse("{}");
  EXPECT_EQ(json->getType(), JSONType::JSONObject);
  EXPECT_EQ(json->as<JSONObject>()->size(), 0);
}

TEST(JSONParse, EmptyArray) {
  JSONBase *json = parse("[]");
  EXPECT_EQ(json->getType(), JSONType::JSONArray);
  EXPECT_EQ(json->as<JSONArray>()->size(), 0);
}

TEST(JSONParse, EmptyString) {
  JSONBase *json = parse("\"\"");
  EXPECT_EQ(json->getType(), JSONType::JSONLiteral);
  EXPECT_EQ(json->as<JSONLiteral>()->value(), "");
}

TEST(JSONParse, EmptyNumber) {
  JSONBase *json = parse("0");
  EXPECT_EQ(json->getType(), JSONType::JSONLiteral);
  EXPECT_EQ(json->as<JSONNumber>()->value(), 0);
}

TEST(JSONParse, EmptyTrue) {
  JSONBase *json = parse("true");
  EXPECT_EQ(json->getType(), JSONType::JSONLiteral);
  EXPECT_TRUE(json->as<JSONBoolean>()->value());
}

TEST(JSONParse, SimpleObject) {
  JSONBase *json = parse("{\"key\": \"value\"}");
  EXPECT_EQ(json->getType(), JSONType::JSONObject);
  EXPECT_EQ(json->as<JSONObject>()->size(), 1);
  EXPECT_EQ(json->as<JSONObject>()->at("key")->getType(),
            JSONType::JSONLiteral);
  EXPECT_EQ(json->as<JSONObject>()->at("key")->as<JSONLiteral>()->value(),
            "value");
}

TEST(JSONParse, SimpleArray) {
  JSONBase *json = parse("[\"value\"]");
  EXPECT_EQ(json->getType(), JSONType::JSONArray);
  EXPECT_EQ(json->as<JSONArray>()->size(), 1);
  EXPECT_EQ(json->as<JSONArray>()->at(0)->getType(), JSONType::JSONLiteral);
  EXPECT_EQ(json->as<JSONArray>()->at(0)->as<JSONLiteral>()->value(), "value");
}

TEST(JSONParse, ObjectNestedArray) {
  JSONBase *json = parse("{\"key\": [\"value\"]}");
  EXPECT_EQ(json->getType(), JSONType::JSONObject);
  EXPECT_EQ(json->as<JSONObject>()->size(), 1);
  EXPECT_EQ(json->as<JSONObject>()->at("key")->getType(),
            JSONType::JSONArray);
  EXPECT_EQ(json->as<JSONObject>()->at("key")->as<JSONArray>()->size(), 1);
  EXPECT_EQ(json->as<JSONObject>()->at("key")->as<JSONArray>()->at(0)->getType(),
            JSONType::JSONLiteral);
  EXPECT_EQ(json->as<JSONObject>()->at("key")->as<JSONArray>()->at(0)->as<
      JSONLiteral>()->value(),
            "value");
}

TEST(JSONParse, ArrayNestedObject) {
  JSONBase *json = parse("[{\"key\": \"value\"}]");
  EXPECT_EQ(json->getType(), JSONType::JSONArray);
  EXPECT_EQ(json->as<JSONArray>()->size(), 1);
  EXPECT_EQ(json->as<JSONArray>()->at(0)->getType(), JSONType::JSONObject);
  EXPECT_EQ(json->as<JSONArray>()->at(0)->as<JSONObject>()->size(), 1);
  EXPECT_EQ(json->as<JSONArray>()->at(0)->as<JSONObject>()->at("key")->getType(),
            JSONType::JSONLiteral);
  EXPECT_EQ(json->as<JSONArray>()->at(0)->as<JSONObject>()->at("key")->as<
      JSONLiteral>()->value(),
            "value");
}

TEST(JSONParse, ArrayMultipleNestedObjects) {
  JSONBase *json = parse("[{\"key\": \"value\"}, {\"key\": \"value\"}]");
  EXPECT_EQ(json->getType(), JSONType::JSONArray);
  EXPECT_EQ(json->as<JSONArray>()->size(), 2);
  EXPECT_EQ(json->as<JSONArray>()->at(0)->getType(), JSONType::JSONObject);
  EXPECT_EQ(json->as<JSONArray>()->at(0)->as<JSONObject>()->size(), 1);
  EXPECT_EQ(json->as<JSONArray>()->at(0)->as<JSONObject>()->at("key")->getType(),
            JSONType::JSONLiteral);
  EXPECT_EQ(json->as<JSONArray>()->at(0)->as<JSONObject>()->at("key")->as<
      JSONLiteral>()->value(),
            "value");
  EXPECT_EQ(json->as<JSONArray>()->at(1)->getType(), JSONType::JSONObject);
  EXPECT_EQ(json->as<JSONArray>()->at(1)->as<JSONObject>()->size(), 1);
  EXPECT_EQ(json->as<JSONArray>()->at(1)->as<JSONObject>()->at("key")->getType(),
            JSONType::JSONLiteral);
  EXPECT_EQ(json->as<JSONArray>()->at(1)->as<JSONObject>()->at("key")->as<
      JSONLiteral>()->value(),
            "value");
}
