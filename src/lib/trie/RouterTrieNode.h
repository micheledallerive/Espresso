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
  RouterTrieNode();
  ~RouterTrieNode();
  [[nodiscard]] bool isLeaf() const;
  [[nodiscard]] bool isRoot() const;

  std::string key;
  std::vector<Route> routes;
  std::vector<std::shared_ptr<RouterTrieNode>> children;
  std::shared_ptr<RouterTrieNode> parent;
};

}

#endif //ESPRESSO_SRC_LIB_TRIE_ROUTERTRIENODE_H_
