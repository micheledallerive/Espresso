//
// Created by michele on 14.02.23.
//

#include "AtomicTransaction.h"
#include "DatabaseManager.h"

namespace Espresso::ORM {

AtomicTransaction::AtomicTransaction() {
  DatabaseManager::getManager()->startTransaction();
}

AtomicTransaction::~AtomicTransaction() {
  if (std::uncaught_exceptions())
    DatabaseManager::getManager()->rollbackTransaction();
  else
    DatabaseManager::getManager()->commitTransaction();
}

} // ORM