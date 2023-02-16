#include <gtest/gtest.h>
#include <orm/sql/SQLGenerator.h>

using namespace Espresso::ORM;

TEST(SQLGenerator, CreateTable) {
  std::vector<BaseFieldData *> fields;
  std::string tableName = "test_table";
  std::string sql = SQLGenerator::createTable(tableName, fields);
  std::string expected = "CREATE TABLE test_table ();";
  ASSERT_EQ(sql, expected);

  fields.push_back(reinterpret_cast<BaseFieldData *>(new FieldData(FieldParams(
      "id",
      true,
      true,
      false,
      std::nullopt))));

  sql = SQLGenerator::createTable(tableName, fields);
  expected =
      "CREATE TABLE test_table (id BLOB AUTOINCREMENT NOT NULL);";
  ASSERT_EQ(sql, expected);

  fields.push_back(reinterpret_cast<BaseFieldData *>(new FieldData(FieldParams(
      "name",
      false,
      false,
      false,
      std::nullopt))));

  sql = SQLGenerator::createTable(tableName, fields);
  expected =
      "CREATE TABLE test_table (id BLOB AUTOINCREMENT NOT NULL, name BLOB);";
  ASSERT_EQ(sql, expected);

  BaseFieldData *data = new FieldData(FieldParams(
      "name",
      false,
      false,
      false,
      std::nullopt));
  data->ctype = typeid(std::string).name();

  fields.push_back(data);
  sql = SQLGenerator::createTable(tableName, fields);
  expected =
      "CREATE TABLE test_table (id BLOB AUTOINCREMENT NOT NULL, name BLOB, name TEXT);";
  ASSERT_EQ(sql, expected);

  data = new FieldData(FieldParams(
      "pk",
      true,
      false,
      false,
      std::nullopt,
      true));

  data->ctype = typeid(int).name();
  fields.push_back(data);

  sql = SQLGenerator::createTable(tableName, fields);
  expected =
      "CREATE TABLE test_table (id BLOB AUTOINCREMENT NOT NULL, name BLOB, name TEXT, pk INTEGER PRIMARY KEY AUTOINCREMENT);";
  ASSERT_EQ(sql, expected);
}