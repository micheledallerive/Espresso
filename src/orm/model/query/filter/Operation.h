//
// Created by michele on 12.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_QUERY_FILTER_OPERATION_H_
#define ESPRESSO_SRC_ORM_MODEL_QUERY_FILTER_OPERATION_H_

#include <string>

namespace Espresso::ORM::Query {

class Operation {
 public:
  virtual std::string toString() const = 0;
};

class RelationalOperator : public Operation {
 public:
  enum Operation {
    EQUAL,
    NOT_EQUAL,
    GREATER_THAN,
    GREATER_THAN_OR_EQUAL,
    LESS_THAN,
    LESS_THAN_OR_EQUAL
  } op;

  explicit RelationalOperator(RelationalOperator::Operation op) : op(op) {}
  std::string toString() const override {
    switch (op) {
      case EQUAL:return "=";
      case NOT_EQUAL:return "!=";
      case GREATER_THAN:return ">";
      case GREATER_THAN_OR_EQUAL:return ">=";
      case LESS_THAN:return "<";
      case LESS_THAN_OR_EQUAL:return "<=";
      default:return "";
    }
  }
};

class LogicalOperator : public Operation {
 public:
  enum Operation {
    AND,
    OR
  } op;

  explicit LogicalOperator(LogicalOperator::Operation op) : op(op) {}
  std::string toString() const override {
    switch (op) {
      case AND:return "AND";
      case OR:return "OR";
      default:return "";
    }
  }
};

} // Query

#endif //ESPRESSO_SRC_ORM_MODEL_QUERY_FILTER_OPERATION_H_
