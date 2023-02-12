//
// Created by michele on 11.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_QUERY_FILTER_FILTEROPERATION_H_
#define ESPRESSO_SRC_ORM_MODEL_QUERY_FILTER_FILTEROPERATION_H_

#include <memory>

#include "FilterNode.h"
#include "Operator.h"

namespace Espresso::ORM::Query {

class FilterOperation : public FilterNode {
 protected:
  std::shared_ptr<FilterNode> left;
  std::shared_ptr<Operator> op;
  std::shared_ptr<FilterNode> right;
 public:
  FilterOperation(FilterNode *left, Operator *op, FilterNode *right)
      : left(left), op(op), right(right) {}

  ~FilterOperation() override = default;

  [[nodiscard]] std::string toString() const override;
  [[nodiscard]] std::vector<std::string> getKeys() const override;
  [[nodiscard]] bool isTerminal() const override;

  FilterOperation operator&(const FilterOperation &) const;
  FilterOperation *operator|(const FilterNode &) const;
};

} // Query

#endif //ESPRESSO_SRC_ORM_MODEL_QUERY_FILTER_FILTEROPERATION_H_
