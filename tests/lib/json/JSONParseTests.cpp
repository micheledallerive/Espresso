//
// Created by michele on 25.02.23.
//

#include <gtest/gtest.h>
#include "lib/json/JSONEntity.h"
#include "lib/json/JSONLiteral.h"
#include "lib/json/JSONArray.h"
#include "lib/json/JSONObject.h"
#include "lib/json/JSONNumber.h"
#include "lib/json/JSONBoolean.h"

using namespace Espresso::JSON;

TEST(JSONParse, EmptyString) {
  std::shared_ptr<JSONEntity> json = JSONEntity::parse("\"\"");
  EXPECT_EQ(json->getType(), JSONType::JSONLiteral);
  EXPECT_EQ(json->as<JSONLiteral>()->value(), "");
}

TEST(JSONParse, String) {
  std::shared_ptr<JSONEntity> json = JSONEntity::parse("\"test\"");
  EXPECT_EQ(json->getType(), JSONType::JSONLiteral);
  EXPECT_EQ(json->as<JSONLiteral>()->value(), "test");
}

TEST(JSONParse, Number) {
  std::shared_ptr<JSONEntity> json = JSONEntity::parse("0");
  EXPECT_EQ(json->getType(), JSONType::JSONNumber);
  EXPECT_EQ(json->as<JSONNumber>()->value(), 0);
}

TEST(JSONParse, DecimalNumber) {
  std::shared_ptr<JSONEntity> json = JSONEntity::parse("1.251");
  EXPECT_EQ(json->getType(), JSONType::JSONNumber);
  EXPECT_EQ(json->as<JSONNumber>()->value(), 1.251);
}

TEST(JSONParse, Boolean) {
  std::shared_ptr<JSONEntity> json = JSONEntity::parse("true");
  EXPECT_EQ(json->getType(), JSONType::JSONBoolean);
  EXPECT_TRUE(json->as<JSONBoolean>()->value());
}

TEST(JSONParse, EmptyArray) {
  std::shared_ptr<JSONEntity> json = JSONEntity::parse("[]");
  EXPECT_EQ(json->getType(), JSONType::JSONArray);
  EXPECT_EQ(json->as<JSONArray>()->size(), 0);
}

TEST(JSONParse, SimpleArray) {
  std::shared_ptr<JSONEntity> json = JSONEntity::parse("[\"value\"]");
  EXPECT_EQ(json->getType(), JSONType::JSONArray);
  EXPECT_EQ(json->as<JSONArray>()->size(), 1);
  EXPECT_EQ(json->as<JSONArray>()->at(0)->getType(), JSONType::JSONLiteral);
  EXPECT_EQ(json->as<JSONArray>()->at(0)->as<JSONLiteral>()->value(), "value");
}

TEST(JSONParse, ArrayNestedArrays) {
  std::string json = R"(["value", ["value2", "value3"], ["value4", "value5"]])";
  std::shared_ptr<JSONEntity> parsed = JSONEntity::parse(json);
  EXPECT_EQ(parsed->getType(), JSONType::JSONArray);
  EXPECT_EQ(parsed->as<JSONArray>()->size(), 3);
}

TEST(JSONParse, EmptyObject) {
  std::shared_ptr<JSONEntity> json = JSONEntity::parse("{}");
  EXPECT_EQ(json->getType(), JSONType::JSONObject);
  EXPECT_EQ(json->as<JSONObject>()->size(), 0);
}

TEST(JSONParse, SimpleObject) {
  auto json = JSONEntity::parse("{\"key\": \"value\"}")->as<JSONObject>();
  EXPECT_EQ(json->getType(), JSONType::JSONObject);
  EXPECT_EQ(json->size(), 1);
  EXPECT_EQ(json->at("key")->getType(),
            JSONType::JSONLiteral);
  EXPECT_EQ(json->at("key")->as<JSONLiteral>()->value(),
            "value");
}

TEST(JSONParse, ObjectNestedArray) {
  std::shared_ptr<JSONEntity> json = JSONEntity::parse("{\"key\": [\"value\"]}");
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
  std::shared_ptr<JSONEntity> json = JSONEntity::parse("[{\"key\": \"value\"}]");
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
  std::shared_ptr<JSONEntity> json = JSONEntity::parse("[{\"key\": \"value\"}, {\"key\": \"value\"}]");
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
