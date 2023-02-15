//
// Created by michele on 18.01.23.
//

#include "SQLiteDatabaseManager.h"
#include <sqlite3.h>
#include "../exceptions.h"
#include "utils.h"
#include "AtomicTransaction.h"
#include "orm/model/fields/FieldData.h"

#include <vector>

namespace Espresso::ORM {

void SQLiteDatabaseManager::connect(const ConnectionOptions &options) {
  auto sqliteOptions = dynamic_cast<const SQLiteConnectionOptions &>(options);
  sqlite3_open(sqliteOptions.databasePath.c_str(), &db);
  if (db == nullptr) {
    throw db_error("Could not connect to database");
  }

  sqlite3_busy_timeout(db, 1000);

  this->connected = true;
}

void SQLiteDatabaseManager::disconnect() {
  sqlite3_close(db);
  this->connected = false;
}

void SQLiteDatabaseManager::execute(const std::string &query,
                                    QueryCallback callback) {
  if (!this->connected) {
    throw db_error("Not connected to database");
  }
  auto pos = query.find_first_of(';');
  if (pos != std::string::npos && pos != query.size() - 1) {
    {
      AtomicTransaction transaction;
      for (auto &q : Espresso::split(query, ';')) {
        execute(q, callback);
      }
    }
    return;
  }

  sqlite3_stmt *stmt;
  const char *sql = query.data();
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    throw sql_error(
        "Could not prepare statement: '" + query + "': "
            + std::string(sqlite3_errmsg(db)));
  }
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    if (!callback) continue;

    std::unordered_map<std::string, std::string> rowResult;
    // for each column
    for (int i = 0; i < sqlite3_column_count(stmt); i++) {
      std::string columnName = sqlite3_column_name(stmt, i);
      const char *columnValue = (const char *) sqlite3_column_text(stmt, i);
      rowResult.emplace(columnName,
                        columnValue == nullptr ? "" : std::string(columnValue));
    }
    callback(rowResult);
  }
  if (callback && query.find("INSERT") != std::string::npos) {
    std::string last_id = std::to_string(sqlite3_last_insert_rowid(db));
    std::unordered_map<std::string, std::string> rowResult;
    rowResult.emplace("id", last_id);
    callback(rowResult);
  }
  if (rc != SQLITE_DONE) {
    throw sql_error("Could not execute statement '" + query + "': " +
        std::string(sqlite3_errmsg(db)));
  }
  sqlite3_finalize(stmt);
}
void SQLiteDatabaseManager::startTransaction() {
  this->execute("BEGIN TRANSACTION", nullptr);
}
void SQLiteDatabaseManager::commitTransaction() {
  this->execute("COMMIT TRANSACTION", nullptr);
}
void SQLiteDatabaseManager::rollbackTransaction() {
  this->execute("ROLLBACK TRANSACTION", nullptr);
}
void SQLiteDatabaseManager::startSavepoint(const std::string &name) {
  this->execute("SAVEPOINT " + name, nullptr);
}
void SQLiteDatabaseManager::releaseSavepoint(const std::string &name) {
  this->execute("RELEASE SAVEPOINT " + name, nullptr);
}
void SQLiteDatabaseManager::rollbackSavepoint(const std::string &name) {
  this->execute("ROLLBACK TO SAVEPOINT " + name, nullptr);
}
std::vector<BaseFieldData *> SQLiteDatabaseManager::getTableFields(const std::string &tableName) {
  std::unordered_map<std::string, BaseFieldData *> fieldsMap;
  this->execute("PRAGMA table_info(" + tableName + ")",
                [&fieldsMap](std::unordered_map<std::string, std::string> row) {
                  auto *fieldData = new BaseFieldData();
                  if (row["type"] == "INTEGER") {
                    fieldData->ctype = typeid(int).name();
                  } else if (row["type"] == "TEXT") {
                    fieldData->ctype = typeid(std::string).name();
                  } else if (row["type"] == "REAL") {
                    fieldData->ctype = typeid(double).name();
                  } else if (row["type"] == "BLOB") {
                    fieldData->ctype = typeid(std::vector<char>).name();
                  } else {
                    throw db_error("Unknown field type: " + row["type"]);
                  }
                  fieldData->name = row["name"];
                  fieldData->primaryKey = row["pk"] == "1";
                  fieldData->notNull = row["notnull"] == "1";
                  fieldData->autoIncrement = row["pk"] == "1";
                  fieldData->defaultValue =
                      row["dflt_value"].empty() ? std::nullopt : std::make_optional(
                          row["dflt_value"]);
                  fieldsMap.emplace(fieldData->name, fieldData);
                });
  // are these fields unique??
  std::vector<std::string> indexes;
  this->execute("PRAGMA index_list(" + tableName + ")",
                [&indexes](std::unordered_map<std::string, std::string> row) {
                  if (row["unique"] == "1") {
                    indexes.push_back(row["name"]);
                  }
                });
  for (auto &index : indexes) {
    std::vector<std::string> indexFields;
    this->execute("PRAGMA index_info(" + index + ")",
                  [&indexFields](std::unordered_map<std::string,
                                                    std::string> row) {
                    indexFields.push_back(row["name"]);
                  });
    for (auto &field : indexFields) {
      fieldsMap[field]->unique = true;
    }
  }
  std::vector<BaseFieldData *> fields;
  for (auto &field : fieldsMap) {
    fields.push_back(field.second);
  }
  return fields;
}

}