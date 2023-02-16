#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <orm/database/AtomicTransaction.h>
#include <orm/database/DatabaseManager.h>
#include <orm/database/SQLiteDatabaseManager.h>

#include <orm/model/query/QueryBuilder.h>
#include <orm/model/query/filter/FilterField.h>
#include <thread>

// first create a table in the database, then test the atomic transaction operating and then when all the tests are done delete the table
using namespace Espresso::ORM;
using namespace Espresso::ORM::Query;

class AtomicTransactionTests : public ::testing::Test {
 protected:
  void SetUp() override {
    SQLiteConnectionOptions options;
    options.databasePath = "test.db";
    DBFactory<SQLiteDatabaseManager>::createAndConnect(options);

    dbManager->execute("DROP TABLE IF EXISTS test");
    dbManager->execute(
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT)");
  }

  void TearDown() override {
    dbManager->execute("DROP TABLE IF EXISTS test");
    dbManager->disconnect();
  }
};

TEST_F(AtomicTransactionTests, ExceptionNotCommitted) {
  // insert a row
  dbManager->execute("INSERT INTO test (name) VALUES ('test')");

  try {
    // start a transaction
    AtomicTransaction transaction;

    // insert a row
    auto query = "INSERT INTO test (name) VALUES ('test2')";
    dbManager->execute(query, nullptr);

    throw std::runtime_error("test");
  } catch (std::exception &e) {
    // check if the row is not inserted
    auto query = "SELECT * FROM test WHERE name = 'test2'";
    int count = 0;
    dbManager->execute(query,
                       [&count](const std::unordered_map<std::string,
                                                         std::string> &result) {
                         count++;
                       });
    ASSERT_EQ(count, 0);
  }

}

TEST_F(AtomicTransactionTests, SuccessfulCommit) {
  // insert a row
  dbManager->execute("INSERT INTO test (name) VALUES ('test')");

  {
    // start a transaction
    AtomicTransaction transaction;

    // insert a row
    auto query = "INSERT INTO test (name) VALUES ('test2')";
    dbManager->execute(query, nullptr);
  }
  // check if the row is inserted
  auto query = "SELECT * FROM test WHERE name = 'test2'";
  int count = 0;
  dbManager->execute(query,
                     [&count](const std::unordered_map<std::string,
                                                       std::string> &result) {
                       count++;
                     });
  ASSERT_EQ(count, 1);
}

TEST_F(AtomicTransactionTests, MultipleTransactions) {
  // insert a row
  dbManager->execute("INSERT INTO test (name) VALUES ('test')");

  {
    // start a transaction
    AtomicTransaction transaction;

    // insert a row
    auto query = "INSERT INTO test (name) VALUES ('test2')";
    dbManager->execute(query, nullptr);
  }
  // check if the row is inserted
  auto query = "SELECT * FROM test WHERE name = 'test2'";
  int count = 0;
  dbManager->execute(query,
                     [&count](const std::unordered_map<std::string,
                                                       std::string> &result) {
                       count++;
                     });
  ASSERT_EQ(count, 1);

  {
    // start a transaction
    AtomicTransaction transaction;

    // insert a row
    auto query = "INSERT INTO test (name) VALUES ('test3')";
    dbManager->execute(query, nullptr);
  }
  // check if the row is inserted
  query = "SELECT * FROM test WHERE name = 'test3'";
  count = 0;
  dbManager->execute(query,
                     [&count](const std::unordered_map<std::string,
                                                       std::string> &result) {
                       count++;
                     });
  ASSERT_EQ(count, 1);
}