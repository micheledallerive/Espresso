//
// Created by michele on 15.01.23.
//

#include "RouterTrieNode.h"
#include "../../Route.h"
#include <vector>
#include <memory>

namespace Espresso {

RouterTrieNode::RouterTrieNode() {
  this->key = "";
  this->routes = std::vector<Route>();
  this->children = std::vector<std::shared_ptr<RouterTrieNode >>();
  this->parent = nullptr;
}

RouterTrieNode::~RouterTrieNode() = default;

bool RouterTrieNode::isLeaf() const {
  return !this->routes.empty();
}

bool RouterTrieNode::isRoot() const {
  return this->parent == nullptr;
}

bool RouterTrieNode::matches(const std::string &otherKey) const {
  return this->key == otherKey
      || (this->key.length() > 1 && this->key[0] == ':')
      || (this->key == "*" && !otherKey.empty());
}

}