//
// Created by michele on 10.01.23.
//
#include <gtest/gtest.h>
#include "requests/cookies.h"

using namespace Espresso;

#define DEFAULT_NAME ""
#define DEFAULT_VALUE ""
#define DEFAULT_DOMAIN ""
#define DEFAULT_PATH "/"
#define DEFAULT_MAX_AGE (60 * 60)

TEST(Cookies, DefaultConstructor) {
  Cookie cookie;
  ASSERT_EQ(cookie.name, DEFAULT_NAME);
  ASSERT_EQ(cookie.value, DEFAULT_VALUE);
  ASSERT_EQ(cookie.domain, DEFAULT_DOMAIN);
  ASSERT_EQ(cookie.path, DEFAULT_PATH);
  ASSERT_EQ(cookie.max_age, DEFAULT_MAX_AGE);
  ASSERT_FALSE(cookie.secure);
  ASSERT_FALSE(cookie.http_only);
}

TEST(Cookies, NameValueConstructor) {
  Cookie cookie("name", "value");
  ASSERT_EQ(cookie.name, "name");
  ASSERT_EQ(cookie.value, "value");
  ASSERT_EQ(cookie.domain, DEFAULT_DOMAIN);
  ASSERT_EQ(cookie.path, DEFAULT_PATH);
  ASSERT_EQ(cookie.max_age, DEFAULT_MAX_AGE);
  ASSERT_FALSE(cookie.secure);
  ASSERT_FALSE(cookie.http_only);

  cookie = Cookie("name", "");
  ASSERT_EQ(cookie.name, "name");
  ASSERT_EQ(cookie.value, "");
}

TEST(Cookies, CompleteConstructor) {
  Cookie cookie(
      "name",
      "value",
      "domain",
      "path",
      420,
      true,
      true
  );
  ASSERT_EQ(cookie.name, "name");
  ASSERT_EQ(cookie.value, "value");
  ASSERT_EQ(cookie.domain, "domain");
  ASSERT_EQ(cookie.path, "path");
  ASSERT_EQ(cookie.max_age, 420);
  ASSERT_TRUE(cookie.secure);
  ASSERT_TRUE(cookie.http_only);
}

//TEST(Cookies, ToString) {
//  Cookie cookie("name", "value", "example.com", "/path", 10, true, true);
//  // domain   path    secure    http_only
//  // 1        1       1         1
//  ASSERT_EQ(cookie.toString(),
//            "name=value; Domain=example.com; Path=/path; Max-Age=10; Secure; HttpOnly");
//  // 1        1       1         0
//  cookie.http_only = false;
//  ASSERT_EQ(cookie.toString(),
//            "name=value; Domain=example.com; Path=/path; Max-Age=10; Secure");
//  // 1        1       0         1
//  cookie.secure = false;
//  cookie.http_only = true;
//  ASSERT_EQ(cookie.toString(),
//            "name=value; Domain=example.com; Path=/path; Max-Age=10; HttpOnly");
//  // 1        1       0         0
//  cookie.http_only = false;
//  ASSERT_EQ(cookie.toString(),
//            "name=value; Domain=example.com; Path=/path; Max-Age=10");
//  // 1        0       1         1
//  cookie.path = "";
//  cookie.secure = true;
//  cookie.http_only = true;
//  ASSERT_EQ(cookie.toString(),
//            "name=value; Domain=example.com; Max-Age=10; Secure; HttpOnly");
//  // 1        0       1         0
//  cookie.http_only = false;
//  ASSERT_EQ(cookie.toString(),
//            "name=value; Domain=example.com; Max-Age=10; Secure");
//  // 1        0       0         1
//  cookie.secure = false;
//  cookie.http_only = true;
//  ASSERT_EQ(cookie.toString(),
//            "name=value; Domain=example.com; Max-Age=10; HttpOnly");
//  // 1        0       0         0
//  cookie.http_only = false;
//  ASSERT_EQ(cookie.toString(),
//            "name=value; Domain=example.com; Max-Age=10");
//
//  // 0        1       1         1
//  cookie.domain = "";
//  cookie.path = "/path";
//  cookie.secure = true;
//  cookie.http_only = true;
//  ASSERT_EQ(cookie.toString(),
//            "name=value; Path=/path; Max-Age=10; Secure; HttpOnly");
//  // 0        1       1         0
//  cookie.http_only = false;
//  ASSERT_EQ(cookie.toString(),
//            "name=value; Path=/path; Max-Age=10; Secure");
//  // 0        1       0         1
//  cookie.secure = false;
//  cookie.http_only = true;
//  ASSERT_EQ(cookie.toString(),
//            "name=value; Path=/path; Max-Age=10; HttpOnly");
//  // 0        1       0         0
//  cookie.http_only = false;
//  ASSERT_EQ(cookie.toString(),
//            "name=value; Path=/path; Max-Age=10");
//  // 0        0       1         1
//  cookie.path = "";
//  cookie.secure = true;
//  cookie.http_only = true;
//  ASSERT_EQ(cookie.toString(),
//            "name=value; Max-Age=10; Secure; HttpOnly");
//  // 0        0       1         0
//  cookie.http_only = false;
//  ASSERT_EQ(cookie.toString(),
//            "name=value; Max-Age=10; Secure");
//  // 0        0       0         1
//  cookie.secure = false;
//  cookie.http_only = true;
//  ASSERT_EQ(cookie.toString(),
//            "name=value; Max-Age=10; HttpOnly");
//  // 0        0       0         0
//  cookie.http_only = false;
//  ASSERT_EQ(cookie.toString(),
//            "name=value; Max-Age=10");
//
//  cookie.name = "";
//  ASSERT_EQ(cookie.toString(), "");
//
//  cookie.value = "";
//  cookie.name = "asdf";
//  ASSERT_EQ(cookie.toString(), "");
//
//  cookie.name = "";
//  ASSERT_EQ(cookie.toString(), "");
//  cookie.name = "name";
//  cookie.value = "value";
//  ASSERT_NE(cookie.toString(), "");
//}

TEST(Cookies, ToString) {
  Cookie cookie("name", "value", "example.com", "/path", 10, true, true);
  ASSERT_EQ(cookie.toString(),
            "name=value; Domain=example.com; Path=/path; Max-Age=10; Secure; HttpOnly");
  cookie.http_only = false;
  ASSERT_EQ(cookie.toString(),
            "name=value; Domain=example.com; Path=/path; Max-Age=10; Secure");
  cookie.secure = false;
  ASSERT_EQ(cookie.toString(),
            "name=value; Domain=example.com; Path=/path; Max-Age=10");
  cookie.path = "";
  ASSERT_EQ(cookie.toString(),
            "name=value; Domain=example.com; Max-Age=10");
  cookie.domain = "";
  ASSERT_EQ(cookie.toString(),
            "name=value; Max-Age=10");

  cookie.name = "";
  ASSERT_EQ(cookie.toString(), "");

  cookie.name = "name";
  cookie.value = "";
  ASSERT_EQ(cookie.toString(), "name=; Max-Age=10");

  cookie.name = "";
  ASSERT_EQ(cookie.toString(), "");
}