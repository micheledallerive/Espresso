//
// Created by michele on 18.01.23.
//

#ifndef ESPRESSO_SRC_ORM_EXCEPTIONS_H_
#define ESPRESSO_SRC_ORM_EXCEPTIONS_H_

#include <stdexcept>

#define create_exception(name) \
  class name : public std::runtime_error \
  {                                                                     \
    public:                                                               \
      explicit name(const std::string &message)                         \
        : std::runtime_error(message) {}; \
  }

namespace Espresso::ORM {

create_exception(sql_error);
create_exception(db_error);
create_exception(model_error);

}

#endif //ESPRESSO_SRC_ORM_EXCEPTIONS_H_
