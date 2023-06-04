//
// Created by michele on 10.01.23.
//

#include <gtest/gtest.h>
#include "Server.h"

namespace Espresso {

TEST(Server, Constructor) {
  Server server;
  EXPECT_EQ(std::any_cast<std::string>(server_settings["BASE_PATH"]),
            ESPRESSO_BASE_PATH);
}

TEST(Server, SettingsConstructor) {
  Settings settings = {
      {"BASE_PATH", std::string("/path")},
      {"TEST", std::string("test")}
  };
  Server server(settings);
  ASSERT_EQ(server_settings.size(), 2);
  EXPECT_EQ(std::any_cast<std::string>(server_settings["BASE_PATH"]),
            "/path");
  EXPECT_EQ(std::any_cast<std::string>(server_settings["TEST"]),
            "test");
}

}