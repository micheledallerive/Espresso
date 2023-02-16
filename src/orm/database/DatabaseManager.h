//
// Created by michele on 18.01.23.
//

#ifndef ESPRESSO_SRC_ORM_DATABASEMANAGER_H_
#define ESPRESSO_SRC_ORM_DATABASEMANAGER_H_

#include <string>
#include <unordered_map>

#include <functional>
#include <memory>
#include "orm/model/fields/FieldData.h"
namespace Espresso::ORM {

using QueryCallback = std::function<void(std::unordered_map<std::string,
                                                            std::string> &)>;

class ConnectionOptions {
 public:
  virtual ~ConnectionOptions() = default;
};

class DatabaseManager;

template<class DM>
class DBFactory;

class DatabaseManager {
 public:
  template<class DM>
  friend class DBFactory;

  DatabaseManager() = default;
  virtual ~DatabaseManager() = default;
  virtual void connect(const ConnectionOptions &options) = 0;
  virtual void disconnect() = 0;
  virtual void execute(const std::string &query,
                       QueryCallback callback = nullptr) = 0;

  virtual std::vector<BaseFieldData *> getTableFields(const std::string &tableName) = 0;

  virtual void startTransaction() = 0;
  virtual void commitTransaction() = 0;
  virtual void rollbackTransaction() = 0;

  virtual void startSavepoint(const std::string &) = 0;
  virtual void releaseSavepoint(const std::string &) = 0;
  virtual void rollbackSavepoint(const std::string &) = 0;

  static std::shared_ptr<DatabaseManager> getManager() {
    return dbManager;
  }

  bool isConnected();
 protected:
  bool connected{false};
 private:
  static std::shared_ptr<DatabaseManager> dbManager;
};

template<class DM>
class DBFactory {
 public:
  static void create() {
    DatabaseManager::dbManager = std::make_shared<DM>();
  }
  static void createAndConnect(const ConnectionOptions &options) {
    create();
    DatabaseManager::getManager()->connect(options);
  }
};

}

#endif //ESPRESSO_SRC_ORM_DATABASEMANAGER_H_
