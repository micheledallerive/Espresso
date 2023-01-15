//
// Created by michele on 14.01.23.
//

#ifndef ESPRESSO_SRC_LIB_TRIE_ROUTERTRIE_H_
#define ESPRESSO_SRC_LIB_TRIE_ROUTERTRIE_H_

#include <string>
#include <functional>
#include "HTTPRequest.h"
#include "HTTPResponse.h"
namespace Espresso {

using RouteCallback = std::function<void(HTTPRequest &request,
                                         HTTPResponse &response)>;

struct Route {
  std::string path{"/"};
  HTTPMethod method{HTTPMethod::GET};
  RouteCallback callback;
};

class RouterTrieNode {
 public:
  RouterTrieNode();
  ~RouterTrieNode();
  [[nodiscard]] bool isLeaf() const;

  std::string key;
  std::vector<Route> routes;
  std::vector<std::shared_ptr<RouterTrieNode>> children;
};

class RouterTrie {
 public:
  explicit RouterTrie(char delimiter);
  ~RouterTrie();

  void insert(const std::string &path, const Route &value);
  bool has(const std::string &path);
  std::vector<Route> search(const std::string &path,
                            HTTPMethod method,
                            std::unordered_map<std::string,
                                               std::string> *params = nullptr);
  size_t size();

 private:
  char delimiter_;
  std::shared_ptr<RouterTrieNode> root_;
  static std::vector<Route> filterByMethod_(std::vector<Route> &routes,
                                            HTTPMethod method);
};

} // Espresso

#endif //ESPRESSO_SRC_LIB_TRIE_ROUTERTRIE_H_
