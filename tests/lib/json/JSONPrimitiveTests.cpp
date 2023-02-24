//
// Created by michele on 24.02.23.
//

#include <gtest/gtest.h>
#include <lib/json/JSONLiteral.h>
#include <lib/json/JSONBoolean.h>

using namespace Espresso::JSON;

TEST(JSONLiteral, toJSON) {
  JSONLiteral literal("test");
  ASSERT_EQ(literal.toJSON(), "\"test\"");
}

TEST(JSONLiteral, toJSONWithQuotesAndEscape) {
  JSONLiteral literal("test\"test");
  ASSERT_EQ(literal.toJSON(), "\"test\"test\"");
}

TEST(JSONBoolean, toJSONTrue) {
  JSONBoolean boolean(true);
  ASSERT_EQ(boolean.toJSON(), "true");
}

TEST(JSONBoolean, toJSONFalse) {
  JSONBoolean boolean(false);
  ASSERT_EQ(boolean.toJSON(), "false");
}


