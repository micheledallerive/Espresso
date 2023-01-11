//
// Created by michele on 10.01.23.
//

#include <gtest/gtest.h>
#include "HTTPResponse.h"
#include "utils.h"
#include "Server.h"
#include "config.h"

namespace Espresso {

TEST(HTTPResponse, Constructor) {
  HTTPResponse response;
  EXPECT_EQ(response.getStatus(), 200);
  EXPECT_EQ(response.getVersion(), "HTTP/1.1");
  EXPECT_EQ(response.getHeader("Date"), getUTCDate());
}

TEST(HTTPResponse, SetStatus) {
  HTTPResponse response;
  response.setStatus(404);
  EXPECT_EQ(response.getStatus(), 404);
  response.setStatus(200);
  EXPECT_EQ(response.getStatus(), 200);
  EXPECT_THROW(response.setStatus(999), std::invalid_argument);
  EXPECT_EQ(response.getStatus(), 200);
}

TEST(HTTPResponse, ToString) {
  HTTPResponse response;
  const std::string dateHeader = "Date: " + getUTCDate() + "\r\n";

  EXPECT_EQ(response.toString(),
            "HTTP/1.1 200 OK\r\n"
                + dateHeader
                + "\r\n");
  response.setBody("Hello World!");
  EXPECT_EQ(response.toString(),
            "HTTP/1.1 200 OK\r\n"
                + dateHeader
                + "\r\n"
                  "Hello World!");
  response.setHeader("Content-Length", "12");
  EXPECT_EQ(response.toString(),
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: 12\r\n"
                + dateHeader
                + "\r\n"
                  "Hello World!");
  response.setStatus(404);
  EXPECT_EQ(response.toString(),
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Length: 12\r\n"
                + dateHeader
                + "\r\n"
                  "Hello World!");
}

TEST(HTTPResponse, Send) {
  HTTPResponse response;
  response.send("Hello World!");
  EXPECT_EQ(response.toString(),
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: 12\r\n"
            "Date: " + getUTCDate() + "\r\n"
                + "\r\n"
                  "Hello World!");
  EXPECT_EQ(response.getBody(), "Hello World!");
}

TEST(HTTPResponse, SendFile) {
  HTTPResponse response;
  response.sendFile(TESTS_PATH + std::string("files/test.html"));
  EXPECT_EQ(response.toString(),
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: 12\r\n"
            "Date: " + getUTCDate() + "\r\n"
                + "\r\n"
                  "Hello World!");
  EXPECT_EQ(response.getBody(), "Hello World!");
  EXPECT_THROW(response.sendFile("tests/does_not_exist.txt"),
               std::runtime_error);
}

TEST(HTTPResponse, DownloadFile) {
  HTTPResponse response;
  response.downloadFile(TESTS_PATH + std::string("files/test.html"));
  EXPECT_EQ(response.toString(),
            "HTTP/1.1 200 OK\r\n"
            "Content-Disposition: attachment\r\n"
            "Content-Length: 12\r\n"
            "Date: " + getUTCDate() + "\r\n"
                + "\r\n"
                  "Hello World!");
  EXPECT_EQ(response.getBody(), "Hello World!");
  response.downloadFile(TESTS_PATH + std::string("files/test.html"),
                        "test.txt");
  EXPECT_EQ(response.toString(),
            "HTTP/1.1 200 OK\r\n"
            "Content-Disposition: attachment; filename=\"test.txt\"\r\n"
            "Content-Length: 12\r\n"
            "Date: " + getUTCDate() + "\r\n"
                + "\r\n"
                  "Hello World!");
  EXPECT_EQ(response.getBody(), "Hello World!");
  EXPECT_THROW(
      response.downloadFile(TESTS_PATH
                                + std::string("files/doesntexist.html"),
                            "test2.html"), std::runtime_error);
}

TEST(HTTPResponse, Redirect) {
  HTTPResponse response;
  const std::string d = getUTCDate();
  response.redirect("http://www.google.com");
  EXPECT_EQ(response.toString(),
            "HTTP/1.1 302 Found\r\n"
            "Date: " + d + "\r\n"
                           "Location: http://www.google.com\r\n"
                + "\r\n");
  EXPECT_EQ(response.getBody(), "");

  response.redirect("http://www.google.com", 301);
  EXPECT_EQ(response.toString(),
            "HTTP/1.1 301 Moved Permanently\r\n"
            "Date: " + d + "\r\n"
                           "Location: http://www.google.com\r\n"
                + "\r\n");
}

TEST(HTTPResponse, SetCookie) {
  HTTPResponse response;
  const std::string date = getUTCDate();
  response.setCookie({"name", "value", "example.com", "/", 3600, true, true});
  EXPECT_EQ(response.toString(),
            "HTTP/1.1 200 OK\r\n"
            "Date: " + date + "\r\n"
                + "Set-Cookie: name=value; Domain=example.com; Path=/; "
                  "Max-Age=3600; Secure; HttpOnly\r\n"
                + "\r\n");
  response = HTTPResponse();
  response.setCookie("name", "value");
  EXPECT_EQ(response.toString(),
            "HTTP/1.1 200 OK\r\n"
            "Date: " + date + "\r\n"
                + "Set-Cookie: name=value; Path=/; Max-Age=3600\r\n"
                + "\r\n");
}

TEST(HTTPResponse, DeleteCookie) {
  HTTPResponse response;
  response.deleteCookie("name");
  EXPECT_EQ(response.toString(),
            "HTTP/1.1 200 OK\r\n"
            "Date: " + getUTCDate() + "\r\n"
                + "Set-Cookie: name=; Max-Age=0\r\n"
                + "\r\n");
}

}