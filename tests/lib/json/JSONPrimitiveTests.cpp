//
// Created by michele on 24.02.23.
//

#include <gtest/gtest.h>
#include "lib/json/json_literal.h"
#include "lib/json/json_boolean.h"
#include "lib/json/json_number.h"

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

TEST(JSONNumber, toJSON) {
  JSONNumber number(1.0);
  ASSERT_EQ(number.toJSON(), "1");
}

TEST(JSONNumber, toJSONWithPrecision) {
  JSONNumber number(1.1234567891234567);
  ASSERT_EQ(number.toJSON(), "1.123456789123457");
}