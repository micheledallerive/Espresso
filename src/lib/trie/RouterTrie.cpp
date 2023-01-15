//
// Created by michele on 14.01.23.
//

#include <queue>
#include "RouterTrie.h"
#include "../../utils.h"

namespace Espresso {

RouterTrie::RouterTrie(char delimiter) {
  this->delimiter_ = delimiter;
  this->root_ = std::make_shared<RouterTrieNode>();
}

RouterTrie::~RouterTrie() = default;

void RouterTrie::insert(const std::string &path, const Route &route) {
  std::vector<std::string> pathParts = split(path, this->delimiter_);
  std::shared_ptr<RouterTrieNode> currentNode = this->root_;
  for (const auto &pathPart : pathParts) {
    bool found = false;
    for (const auto &child : currentNode->children) {
      if (child->key == pathPart) {
        currentNode = child;
        found = true;
        break;
      }
    }
    if (!found) {
      auto newNode = std::make_shared<RouterTrieNode>();
      newNode->key = pathPart;
      newNode->parent = currentNode;
      currentNode->children.push_back(newNode);
      currentNode = newNode;
    }
  }
  currentNode->routes.push_back(route);
}

bool RouterTrie::has(const std::string &path) {
  return !this->search(path, HTTPMethod::ALL).empty();
}

std::vector<Route> RouterTrie::search(const std::string &path,
                                      HTTPMethod method,
                                      std::unordered_map<std::string,
                                                         std::string> *params) {
  std::vector<std::string> pathParts = split(path, this->delimiter_);
  std::shared_ptr<RouterTrieNode> currentNode = this->root_;
  for (auto &pathPart : pathParts) {
    bool found = false;
    bool _exit = false;
    for (const auto &child : currentNode->children) {
      if (child->key == pathPart
          || child->key == "*"
          || child->key.length() > 1 && child->key[0] == ':') {
        currentNode = child;
        found = true;
        if (child->key.length() > 1 && child->key[0] == ':'
            && params != nullptr) {
          (*params)[child->key.substr(1)] = pathPart;
        }
        if (child->key == "*" && child->isLeaf()) {
          _exit = true;
        }
        break;
      }
    }
    if (_exit) break;
    if (!found) {
      return {};
    }
  }
  return filterByMethod_(currentNode->routes, method);
}

std::vector<Route> RouterTrie::filterByMethod_(std::vector<Route> &routes,
                                               HTTPMethod method) {
  std::vector<Route> filteredRoutes;
  for (auto &route : routes) {
    if (route.method == method
        || route.method == HTTPMethod::ALL
        || method == HTTPMethod::ALL) {
      filteredRoutes.push_back(route);
    }
  }
  return filteredRoutes;
}

size_t RouterTrie::size() {
  size_t size = 0;
  std::queue<std::shared_ptr<RouterTrieNode>> queue;
  queue.push(this->root_);
  while (!queue.empty()) {
    auto currentNode = queue.front();
    queue.pop();
    size += currentNode->routes.size();
    for (const auto &child : currentNode->children) {
      queue.push(child);
    }
  }
  return size;
}

} // Espresso