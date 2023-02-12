//
// Created by michele on 11.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_QUERY_FILTER_FILTEROPERATION_H_
#define ESPRESSO_SRC_ORM_MODEL_QUERY_FILTER_FILTEROPERATION_H_

#include <memory>

#include "FilterNode.h"

namespace Espresso::ORM::Query {

enum class Operation {
  EQUAL,
  NOT_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,
};

std::string operatorToString(Operation op);

class FilterOperation : public FilterNode {
 protected:
  std::unique_ptr<FilterNode> left;
  Operation op;
  std::unique_ptr<FilterNode> right;
 public:
  FilterOperation(FilterNode *left, Operation op, FilterNode *right)
      : left(left), op(op), right(right) {}

  ~FilterOperation() override = default;

  std::string toString() const override;
};

} // Query

#endif //ESPRESSO_SRC_ORM_MODEL_QUERY_FILTER_FILTEROPERATION_H_
