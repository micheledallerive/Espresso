//
// Created by michele on 10.01.23.
//

#include <gtest/gtest.h>
#include "requests/HTTPMethod.h"

TEST(HTTPMethod, MethodToString) {
  EXPECT_EQ("GET", Espresso::methodToString(Espresso::GET));
  EXPECT_EQ("POST", Espresso::methodToString(Espresso::POST));
  EXPECT_EQ("PUT", Espresso::methodToString(Espresso::PUT));
  EXPECT_EQ("DELETE", Espresso::methodToString(Espresso::DELETE));
  EXPECT_EQ("HEAD", Espresso::methodToString(Espresso::HEAD));
  EXPECT_EQ("OPTIONS", Espresso::methodToString(Espresso::OPTIONS));
  EXPECT_EQ("PATCH", Espresso::methodToString(Espresso::PATCH));
  EXPECT_EQ("TRACE", Espresso::methodToString(Espresso::TRACE));
  EXPECT_EQ("CONNECT", Espresso::methodToString(Espresso::CONNECT));
  EXPECT_EQ("UNDEFINED", Espresso::methodToString(Espresso::UNDEFINED));
  EXPECT_EQ("ALL", Espresso::methodToString(Espresso::ALL));
}

TEST(HTTPMethod, StringToMethod) {
  EXPECT_EQ(Espresso::GET, Espresso::stringToMethod("GET"));
  EXPECT_EQ(Espresso::POST, Espresso::stringToMethod("POST"));
  EXPECT_EQ(Espresso::PUT, Espresso::stringToMethod("PUT"));
  EXPECT_EQ(Espresso::DELETE, Espresso::stringToMethod("DELETE"));
  EXPECT_EQ(Espresso::HEAD, Espresso::stringToMethod("HEAD"));
  EXPECT_EQ(Espresso::OPTIONS, Espresso::stringToMethod("OPTIONS"));
  EXPECT_EQ(Espresso::PATCH, Espresso::stringToMethod("PATCH"));
  EXPECT_EQ(Espresso::TRACE, Espresso::stringToMethod("TRACE"));
  EXPECT_EQ(Espresso::CONNECT, Espresso::stringToMethod("CONNECT"));
  EXPECT_EQ(Espresso::UNDEFINED, Espresso::stringToMethod("UNDEFINED"));
  EXPECT_EQ(Espresso::ALL, Espresso::stringToMethod("ALL"));
}