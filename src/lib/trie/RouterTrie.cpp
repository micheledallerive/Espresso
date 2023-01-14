//
// Created by michele on 14.01.23.
//

#include "RouterTrie.h"
#include "../../utils.h"

namespace Espresso {

RouterTrieNode::RouterTrieNode() {
  this->key = "";
  this->routes = std::vector<Route>();
  this->children = std::vector<RouterTrieNode *>();
  this->terminal = false;
}

RouterTrieNode::~RouterTrieNode() {
  for (auto child : this->children) {
    delete child;
  }
}

RouterTrie::RouterTrie(char delimiter) {
  this->delimiter_ = delimiter;
  this->root_ = new RouterTrieNode();
}

RouterTrie::~RouterTrie() = default;

void RouterTrie::insert(const std::string &path, const Route &route) {
  std::vector<std::string> pathParts = split(path, this->delimiter_);
  RouterTrieNode *currentNode = this->root_;
  for (auto &pathPart : pathParts) {
    bool found = false;
    for (auto child : currentNode->children) {
      if (child->key == pathPart) {
        currentNode = child;
        found = true;
        break;
      }
    }
    if (!found) {
      auto *newNode = new RouterTrieNode();
      newNode->key = pathPart;
      currentNode->children.push_back(newNode);
      currentNode = newNode;
    }
  }
  currentNode->terminal = true;
  currentNode->routes.push_back(route);
}

bool RouterTrie::has(const std::string &path) {
  std::vector<std::string> pathParts = split(path, this->delimiter_);
  RouterTrieNode *currentNode = this->root_;
  for (auto &pathPart : pathParts) {
    bool found = false;
    for (auto child : currentNode->children) {
      if (child->key == pathPart) {
        currentNode = child;
        found = true;
        break;
      }
    }
    if (!found) {
      return false;
    }
  }
  return currentNode->terminal;
}

std::vector<Route> RouterTrie::search(const std::string &path) {
  std::vector<std::string> pathParts = split(path, this->delimiter_);
  RouterTrieNode *currentNode = this->root_;
  for (auto &pathPart : pathParts) {
    bool found = false;
    for (auto child : currentNode->children) {
      if (child->key == pathPart) {
        currentNode = child;
        found = true;
        break;
      }
    }
    if (!found) {
      return {};
    }
  }
  return currentNode->routes;
}

} // Espresso