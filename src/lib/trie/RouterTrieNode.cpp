//
// Created by michele on 15.01.23.
//

#include "RouterTrieNode.h"
#include "../../Route.h"
#include "../../PathRegex.h"
#include <vector>
#include <memory>

namespace Espresso {

RouterTrieNode::RouterTrieNode(size_t depth) {
  this->key = "";
  this->routes = std::vector<Route>();
  this->children = std::vector<std::shared_ptr<RouterTrieNode >>();
  this->parent = nullptr;
  this->depth = depth;
}

RouterTrieNode::~RouterTrieNode() = default;

bool RouterTrieNode::isLeaf() const {
  return !this->routes.empty();
}

bool RouterTrieNode::isRoot() const {
  return this->parent == nullptr;
}

bool RouterTrieNode::matches(const std::string &otherKey) const {
  return PathRegex::urlPartMatch(this->key, otherKey);
}

}