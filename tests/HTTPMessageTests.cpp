//
// Created by michele on 10.01.23.
//

#include <gtest/gtest.h>
#include <HTTPMessage.h>

namespace Espresso {

HTTPMessage generate() {
  return {
      "HTTP/1.0",
      "Content-Type: text/plain\r\nContent-Length: 420\r\n",
      "Hello World!"
  };
}

TEST(HTTPMessage, DefaultConstructor) {
  HTTPMessage message;
  EXPECT_EQ(message.getVersion(), "");
  EXPECT_EQ(message.getBody(), "");
}

TEST(HTTPMessage, Constructor) {
  auto message = generate();
  EXPECT_EQ(message.getVersion(), "HTTP/1.0");
  EXPECT_EQ(message.getBody(), "Hello World!");
  EXPECT_EQ(message.getHeader("Content-Type"), "text/plain");
}

TEST(HTTPMessage, ParseHeaders) {
  HTTPMessage message;
  message.parseHeaders_("Content-Type: text/html\r\nContent-Length: 420\r\n");
  EXPECT_EQ(message.getHeader("Content-Type"), "text/html");
  EXPECT_EQ(message.getHeader("Content-Length"), "420");
  message.parseHeaders_("");

  message = HTTPMessage();
  message.parseHeaders_("Content-Type: text/html\r\nContent-Length test\r\n");
  EXPECT_EQ(message.getHeader("Content-Type"), "text/html");
  EXPECT_FALSE(message.hasHeader("Content-Length"));

  message = HTTPMessage();
  message.parseHeaders_("Test: prova\r\nEmpty:");
  EXPECT_EQ(message.getHeader("Test"), "prova");
  EXPECT_FALSE(message.hasHeader("Empty"));
}

TEST(HTTPMessage, SetHeader) {
  HTTPMessage message;
  message.setHeader("test", "test1");
  EXPECT_TRUE(message.hasHeader("test"));
  EXPECT_EQ(message.getHeader("test"), "test1");
}

TEST(HTTPMessage, HasHeader) {
  HTTPMessage message;
  message.setHeader("test", "test");
  EXPECT_TRUE(message.hasHeader("test"));
  EXPECT_FALSE(message.hasHeader("nottest"));
}

TEST(HTTPMessage, GetHeader) {
  HTTPMessage message;
  message.setHeader("test", "test");
  EXPECT_EQ(message.getHeader("test"), "test");
  EXPECT_THROW(message.getHeader("nottest"), std::out_of_range);
}

TEST(HTTPMessage, SetBody) {
  HTTPMessage message;
  message.setBody("test");
  EXPECT_EQ(message.getBody(), "test");
}

TEST(HTTPMessage, GetBody) {
  HTTPMessage message;
  message.setBody("test");
  EXPECT_EQ(message.getBody(), "test");
}

TEST(HTTPMessage, HeadersToString) {
  HTTPMessage message;
  message.setHeader("Content-Type", "text/plain");
  message.setHeader("Content-Length", "420");
  EXPECT_EQ(message.headersToString(),
            "Content-Length: 420\r\nContent-Type: text/plain\r\n");
}

} // namespace Espresso