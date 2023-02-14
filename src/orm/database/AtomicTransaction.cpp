//
// Created by michele on 14.02.23.
//

#include "AtomicTransaction.h"
#include "DatabaseManager.h"

namespace Espresso::ORM {

AtomicTransaction::AtomicTransaction() {
  dbManager->startTransaction();
}

AtomicTransaction::~AtomicTransaction() {
  if (std::uncaught_exceptions())
    dbManager->rollbackTransaction();
  else
    dbManager->commitTransaction();
}

} // ORM