//
// Created by michele on 10.01.23.
//

#include <gtest/gtest.h>
#include <HTTPMethod.h>

using namespace Espresso;

TEST(HTTPMethod, MethodToString) {
  EXPECT_EQ("GET", methodToString(GET));
  EXPECT_EQ("POST", methodToString(POST));
  EXPECT_EQ("PUT", methodToString(PUT));
  EXPECT_EQ("DELETE", methodToString(DELETE));
  EXPECT_EQ("HEAD", methodToString(HEAD));
  EXPECT_EQ("OPTIONS", methodToString(OPTIONS));
  EXPECT_EQ("PATCH", methodToString(PATCH));
  EXPECT_EQ("TRACE", methodToString(TRACE));
  EXPECT_EQ("CONNECT", methodToString(CONNECT));
  EXPECT_EQ("UNDEFINED", methodToString(UNDEFINED));
  EXPECT_EQ("ALL", methodToString(ALL));
}

TEST(HTTPMethod, StringToMethod) {
  EXPECT_EQ(GET, stringToMethod("GET"));
  EXPECT_EQ(POST, stringToMethod("POST"));
  EXPECT_EQ(PUT, stringToMethod("PUT"));
  EXPECT_EQ(DELETE, stringToMethod("DELETE"));
  EXPECT_EQ(HEAD, stringToMethod("HEAD"));
  EXPECT_EQ(OPTIONS, stringToMethod("OPTIONS"));
  EXPECT_EQ(PATCH, stringToMethod("PATCH"));
  EXPECT_EQ(TRACE, stringToMethod("TRACE"));
  EXPECT_EQ(CONNECT, stringToMethod("CONNECT"));
  EXPECT_EQ(UNDEFINED, stringToMethod("UNDEFINED"));
  EXPECT_EQ(ALL, stringToMethod("ALL"));
}