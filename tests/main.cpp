//
// Created by michele on 08.01.23.
//

#include <gtest/gtest.h>
#include "../src/HTTPMessage.h"

TEST(HTTPMessage, testParseHeaders) {
  std::string headers = "Content-Type: text/html\r\n"
                        "Content-Length: 123\r\n"
                        "Connection: keep-alive\r\n";
  Espresso::HTTPMessage message("HTTP/1.1", headers, "");
  ASSERT_EQ(message.getHeader("Content-Type"), "text/html");
}