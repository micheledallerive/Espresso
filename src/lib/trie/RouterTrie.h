//
// Created by michele on 14.01.23.
//

#ifndef ESPRESSO_SRC_LIB_TRIE_ROUTERTRIE_H_
#define ESPRESSO_SRC_LIB_TRIE_ROUTERTRIE_H_

#include <string>
#include <functional>
#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "RouterTrieNode.h"

namespace Espresso {

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
