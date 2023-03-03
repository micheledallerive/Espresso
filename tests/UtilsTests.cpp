//
// Created by michele on 10.01.23.
//

#include <gtest/gtest.h>
#include "utils.cpp"

namespace Espresso {

TEST(Utils, Split) {
  std::string test_string = "Hello World";
  std::vector<std::string> split_string = split(test_string, ' ');
  ASSERT_EQ(split_string.size(), 2);
  ASSERT_EQ(split_string[0], "Hello");
  ASSERT_EQ(split_string[1], "World");

  test_string = "Hello,World,Test,";
  std::vector<std::string> split_string2 = split(test_string, ',');
  ASSERT_EQ(split_string2.size(), 3);
  ASSERT_EQ(split_string2[0], "Hello");
  ASSERT_EQ(split_string2[1], "World");
  ASSERT_EQ(split_string2[2], "Test");

}

TEST(Utils, GetUTCDate) {
  std::string d = getUTCDate();
  ASSERT_EQ(d.length(), 29);

  std::vector<std::string> parts = split(d, ' ');
  ASSERT_EQ(parts.size(), 6);

  ASSERT_EQ(parts[0].back(), ',');
  ASSERT_TRUE(isupper(parts[0][0]));
  ASSERT_TRUE(islower(parts[0][1]));
  ASSERT_TRUE(islower(parts[0][2]));

  ASSERT_EQ(parts[1].length(), 2);

  ASSERT_EQ(parts[2].length(), 3);
  ASSERT_TRUE(isupper(parts[2][0]));
  ASSERT_TRUE(islower(parts[2][1]));
  ASSERT_TRUE(islower(parts[2][2]));

  ASSERT_EQ(parts[3].length(), 4);

  ASSERT_EQ(parts[4].length(), 8);

  ASSERT_EQ(parts[5], "GMT");
}

TEST(Utils, GetAbsolutePath) {
  Settings settings = {
      {"BASE_PATH", std::string("/test/")}
  };
  Server server(settings);
  ASSERT_EQ(getAbsolutePath("/test"), "/test");
  ASSERT_EQ(getAbsolutePath("/test/"), "/test/");
  ASSERT_EQ(getAbsolutePath("/test/test"), "/test/test");

  ASSERT_EQ(getAbsolutePath("hello"), "/test/hello");
  ASSERT_EQ(getAbsolutePath("hello/"), "/test/hello/");
  ASSERT_EQ(getAbsolutePath("hello/test"), "/test/hello/test");
}

TEST(Utils, URLDecode) {
  ASSERT_EQ(urlDecode("Hello"), "Hello");
  ASSERT_EQ(urlDecode("Hello%20World"), "Hello World");
  // test all the encoded characters
  ASSERT_EQ(urlDecode("Hello%20World%21"), "Hello World!");
  ASSERT_EQ(urlDecode("Hello%20World%22"), "Hello World\"");
  ASSERT_EQ(urlDecode("Hello%20World%23"), "Hello World#");
  ASSERT_EQ(urlDecode("Hello%20World%24"), "Hello World$");
  ASSERT_EQ(urlDecode("Hello%20World%25"), "Hello World%");
}

}