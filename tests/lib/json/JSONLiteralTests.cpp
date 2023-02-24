//
// Created by michele on 24.02.23.
//

#include <gtest/gtest.h>
#include <lib/json/JSONLiteral.h>

using namespace Espresso::JSON;

TEST(JSONLiteral, toJSON) {
  JSONLiteral literal("test");
  ASSERT_EQ(literal.toJSON(), "\"test\"");
}

TEST(JSONLiteral, toJSONWithQuotesAndEscape) {
  JSONLiteral literal("test\"test");
  ASSERT_EQ(literal.toJSON(), "\"test\"test\"");
}
