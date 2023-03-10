//
// Created by michele on 15.01.23.
//

#include <gtest/gtest.h>
#include "PathRegex.h"

using namespace Espresso;

TEST(PathRegex, PathToRegex) {
  EXPECT_EQ(PathRegex::pathToRegex("/test"), "/test");
  EXPECT_EQ(PathRegex::pathToRegex("/test/:id"), "/test/([^/]+)");

  EXPECT_EQ(PathRegex::pathToRegex("/*"), "/.*");
  EXPECT_EQ(PathRegex::pathToRegex("/test/*"), "/test/.*");
  EXPECT_EQ(PathRegex::pathToRegex("/test/:id/*"), "/test/([^/]+)/.*");
  EXPECT_EQ(PathRegex::pathToRegex("/test/:id/*/:id"),
            "/test/([^/]+)/.*/([^/]+)");

  EXPECT_EQ(PathRegex::pathToRegex("/test/ab?c"), "/test/ab.?c");
  EXPECT_EQ(PathRegex::pathToRegex("/test/ab+c"), "/test/ab.+c");
}

TEST(PathRegex, UrlsMatch) {
  const auto f = [](const std::string &schema, const std::string &url) {
    return PathRegex::urlsMatch(PathRegex::pathToRegex(schema), url);
  };
  EXPECT_TRUE(f("/test", "/test"));
  EXPECT_FALSE(f("/test", "/test2"));

  EXPECT_TRUE(f("/test/:id", "/test/1"));
  EXPECT_TRUE(f("/test/:id", "/test/2"));
  EXPECT_FALSE(f("/test/:id", "/test2/1"));
  EXPECT_FALSE(f("/test/:id", "/test/2/2"));

  EXPECT_TRUE(f("/*", "/"));
  EXPECT_TRUE(f("/*", "/hello"));
  EXPECT_TRUE(f("/*", "/hello/world"));

  EXPECT_TRUE(f("/test/*", "/test/1"));
  EXPECT_TRUE(f("/test/*", "/test/2"));
  EXPECT_TRUE(f("/test/*", "/test/2/2"));
  EXPECT_TRUE(f("/test/*", "/test/2/2/2"));
  EXPECT_FALSE(f("/test/*", "/test2/1"));

  EXPECT_TRUE(f("/test/:id/*", "/test/1/2"));
  EXPECT_TRUE(f("/test/:id/*", "/test/1/2/3"));

  EXPECT_FALSE(f("/test/t+", "/test/t"));
  EXPECT_TRUE(f("/test/t+", "/test/tt"));
  EXPECT_TRUE(f("/test/t+", "/test/test"));
  EXPECT_FALSE(f("/test/t+", "/test/"));

  EXPECT_FALSE(f("/test/t*", "/test/"));
  EXPECT_TRUE(f("/test/t*", "/test/t"));
  EXPECT_TRUE(f("/test/t*", "/test/tt"));
  EXPECT_TRUE(f("/test/t*", "/test/test"));

  EXPECT_TRUE(f("/test/t?st", "/test/tst"));
  EXPECT_TRUE(f("/test/t?st", "/test/test"));
  EXPECT_TRUE(f("/test/t?st", "/test/tast"));
  EXPECT_FALSE(f("/test/t?st", "/test/tttst"));
  EXPECT_FALSE(f("/test/t?st", "/test/"));
}