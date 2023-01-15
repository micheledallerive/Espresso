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
      auto newNode = std::make_shared<RouterTrieNode>(currentNode->depth + 1);
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
  std::queue<std::shared_ptr<RouterTrieNode>> currentLevel;
  std::queue<std::shared_ptr<RouterTrieNode>> nextLevel;
  currentLevel.push(this->root_);
  std::vector<Route> routes;
  std::shared_ptr<RouterTrieNode> result = nullptr;
  for (const auto &pathPart : pathParts) {
    while (!currentLevel.empty()) {
      auto currentNode = currentLevel.front();
      currentLevel.pop();
      for (const auto &child : currentNode->children) {
        if (child->matches(pathPart)) {
          if (child->key == "*") {
            result = child;
            goto result;
          }
          nextLevel.push(child);
        }
      }
    }
    currentLevel = nextLevel;
    while (!nextLevel.empty()) {
      nextLevel.pop();
    }
  }
  if (!currentLevel.empty()) result = currentLevel.front();

  result:
  if (result != nullptr) {
    routes = result->routes;
    if (params != nullptr) {
      auto current = result;
      while (current != nullptr) {
        if (current->key[0] == ':') {
          (*params)[current->key.substr(1)] = pathParts[current->depth - 1];
        }
        current = current->parent;
      }
    }
  }
  return filterByMethod_(routes, method);
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