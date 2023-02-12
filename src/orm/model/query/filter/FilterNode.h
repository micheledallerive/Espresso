//
// Created by michele on 11.02.23.
//

#ifndef ESPRESSO_SRC_ORM_MODEL_QUERY_FILTER_FILTERNODE_H_
#define ESPRESSO_SRC_ORM_MODEL_QUERY_FILTER_FILTERNODE_H_

#include <string>
#include <vector>

namespace Espresso::ORM::Query {

class FilterNode {
 public:
  virtual ~FilterNode() = default;

  [[nodiscard]] virtual std::string toString() const {
    return "";
  }
  [[nodiscard]] virtual std::vector<std::string> getKeys() const {
    return {};
  }
  [[nodiscard]] virtual bool isTerminal() const {
    return false;
  }
};

} // Query

#endif //ESPRESSO_SRC_ORM_MODEL_QUERY_FILTER_FILTERNODE_H_
