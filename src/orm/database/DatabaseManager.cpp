//
// Created by michele on 07.02.23.
//

#include <orm/database/DatabaseManager.h>

namespace Espresso::ORM {
std::shared_ptr<DatabaseManager> DatabaseManager::dbManager = nullptr;

bool DatabaseManager::isConnected() {
  return this->connected;
}
}