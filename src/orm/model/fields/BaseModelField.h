//
// Created by michele on 09.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_FIELDS_BASEMODELFIELD_H_
#define ESPRESSO_SRC_ORM_MODEL_FIELDS_BASEMODELFIELD_H_

namespace Espresso::ORM {

namespace Query {
template<typename G>
class QueryBuilder;
}

class BaseModelField {
 public:
  template<typename G>
  friend
  class Query::QueryBuilder;
  virtual ~BaseModelField() = default;
 protected:
  bool dirty{false};
  void setDirty(bool);
};

} // ORM

#endif //ESPRESSO_SRC_ORM_MODEL_FIELDS_BASEMODELFIELD_H_
