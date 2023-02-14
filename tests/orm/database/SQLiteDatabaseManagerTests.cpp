//
// Created by michele on 08.02.23.
//
#include <gtest/gtest.h>
#include "orm/database/SQLiteDatabaseManager.h"

using namespace Espresso::ORM;

TEST(SQLiteDatabaseManagerTest, Connect) {
  SQLiteDatabaseManager dbManager;
  SQLiteConnectionOptions options;
  options.databasePath = "test.db";
  try {
    dbManager.connect(options);
    ASSERT_TRUE(dbManager.isConnected());
  } catch (std::exception &e) {
    FAIL() << "Unexpected exception: " << e.what();
  }
}

TEST(SQLiteDatabaseManagerTest, Execute) {
  SQLiteDatabaseManager dbManager;
  SQLiteConnectionOptions options;
  options.databasePath = "test.db";
  dbManager.connect(options);

  std::string query =
      "CREATE TABLE IF NOT EXISTS test(id INT PRIMARY KEY NOT NULL, name TEXT NOT NULL)";
  try {
    dbManager.execute(query, nullptr);
    query = "INSERT INTO test (id, name) VALUES (1, 'John Doe')";
    dbManager.execute(query, nullptr);
    query = "SELECT * FROM test";
    dbManager.execute(query,
                      [](const std::unordered_map<std::string,
                                                  std::string> &result) {
                        ASSERT_EQ(result.at("id"), "1");
                        ASSERT_EQ(result.at("name"), "John Doe");
                      });

    query = "DROP TABLE test";
    dbManager.execute(query, nullptr);

  } catch (std::exception &e) {
    FAIL() << "Unexpected exception: " << e.what();
  }
}

TEST(SQLiteDatabaseManagerTest, Disconnect) {
  SQLiteDatabaseManager dbManager;
  SQLiteConnectionOptions options;
  options.databasePath = "test.db";
  dbManager.connect(options);
  dbManager.disconnect();
  ASSERT_FALSE(dbManager.isConnected());
}

TEST(SqliteDatabaseManagerTest, GetTableFields) {
  SQLiteDatabaseManager dbManager;
  SQLiteConnectionOptions options;
  options.databasePath = "test.db";
  dbManager.connect(options);

  std::string query = "DROP TABLE IF EXISTS test";
  try {
    dbManager.execute(query, nullptr);
    query =
        "CREATE TABLE IF NOT EXISTS test(id INT PRIMARY KEY NOT NULL, name TEXT NOT NULL)";
    dbManager.execute(query, nullptr);
    query = "INSERT INTO test (id, name) VALUES (1, 'John Doe')";
    dbManager.execute(query, nullptr);
    query = "SELECT * FROM test";
    auto fields = dbManager.getTableFields("test");
    ASSERT_EQ(fields.size(), 2);

    EXPECT_EQ(fields[0].name, "id");
    EXPECT_EQ(fields[0].ctype, typeid(int).name());
    EXPECT_EQ(fields[0].primaryKey, true);
    EXPECT_EQ(fields[0].notNull, true);

    EXPECT_EQ(fields[1].name, "name");
    EXPECT_EQ(fields[1].ctype, typeid(std::string).name());
    EXPECT_EQ(fields[1].primaryKey, false);
    EXPECT_EQ(fields[1].notNull, true);

    query = "DROP TABLE test";
    dbManager.execute(query, nullptr);

  } catch (std::exception &e) {
    FAIL() << "Unexpected exception: " << e.what();
  }
}
