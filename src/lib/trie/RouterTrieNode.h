//
// Created by michele on 15.01.23.
//

#ifndef ESPRESSO_SRC_LIB_TRIE_ROUTERTRIENODE_H_
#define ESPRESSO_SRC_LIB_TRIE_ROUTERTRIENODE_H_

#include <vector>
#include <string>
#include "../../Route.h"

namespace Espresso {

class RouterTrieNode {
 public:
  explicit RouterTrieNode(size_t depth = 0);
  ~RouterTrieNode();
  [[nodiscard]] bool isLeaf() const;
  [[nodiscard]] bool isRoot() const;
  [[nodiscard]] bool matches(const std::string &otherKey) const;

  std::string key;
  std::vector<Route> routes;
  std::vector<std::shared_ptr<RouterTrieNode>> children;
  std::shared_ptr<RouterTrieNode> parent;
  size_t depth;
};

}

#endif //ESPRESSO_SRC_LIB_TRIE_ROUTERTRIENODE_H_
