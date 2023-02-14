//
// Created by michele on 14.02.23.
//

#ifndef ESPRESSO_SRC_ORM_DATABASE_ATOMICTRANSACTION_H_
#define ESPRESSO_SRC_ORM_DATABASE_ATOMICTRANSACTION_H_

namespace Espresso::ORM {

class AtomicTransaction {
 public:
  AtomicTransaction();
  ~AtomicTransaction();

  AtomicTransaction &operator=(const AtomicTransaction &) = delete;
  AtomicTransaction &operator=(AtomicTransaction &&) = delete;
  AtomicTransaction(const AtomicTransaction &) = delete;
  AtomicTransaction(AtomicTransaction &&) = delete;
};

} // ORM

#endif //ESPRESSO_SRC_ORM_DATABASE_ATOMICTRANSACTION_H_
