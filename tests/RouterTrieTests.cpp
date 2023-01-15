//
// Created by michele on 14.01.23.
//

#include <gtest/gtest.h>
#include "lib/trie/RouterTrie.h"

namespace Espresso {

TEST(RouterTrie, Insert) {
  RouterTrie trie = RouterTrie('/');
  Route route = Route();
  route.path = "/test";
  route.method = HTTPMethod::GET;
  route.callback = [](HTTPRequest &request, HTTPResponse &response) {};
  trie.insert(route.path, route);
  ASSERT_TRUE(trie.has(route.path));
  ASSERT_EQ(trie.size(), 1);
}

TEST(RouterTrie, InsertMultiple) {
  RouterTrie trie = RouterTrie('/');
  trie.insert("/test", Route());
  trie.insert("/test", Route());
  trie.insert("/hello", Route());
  trie.insert("/hello/world", Route());
  ASSERT_EQ(trie.size(), 4);
  ASSERT_TRUE(trie.has("/test"));
  ASSERT_TRUE(trie.has("/hello"));
  ASSERT_TRUE(trie.has("/hello/world"));
}

TEST(RouterTrie, Search) {
  RouterTrie trie = RouterTrie('/');
  Route route = Route();
  route.path = "/test";
  route.method = HTTPMethod::GET;
  route.callback = [](HTTPRequest &request, HTTPResponse &response) {};
  trie.insert(route.path, route);
  std::vector<Route> routes = trie.search(route.path, GET);
  ASSERT_EQ(routes.size(), 1);
  ASSERT_EQ(routes[0].path, route.path);
  ASSERT_EQ(routes[0].method, route.method);
  ASSERT_EQ(routes[0].callback.target_type().name(),
            route.callback.target_type().name());
}

TEST(RouterTrie, SearchWithParams) {
  RouterTrie trie = RouterTrie('/');
  Route route = Route();
  route.path = "/test/:id";
  route.method = HTTPMethod::GET;
  route.callback = [](HTTPRequest &request, HTTPResponse &response) {};
  trie.insert(route.path, route);
  std::vector<Route> routes = trie.search("/test/123", GET);
  ASSERT_EQ(routes.size(), 1);
  ASSERT_EQ(routes[0].path, route.path);
  ASSERT_EQ(routes[0].method, route.method);
  ASSERT_EQ(routes[0].callback.target_type().name(),
            route.callback.target_type().name());
}

TEST(RouterTrie, SearchWithWildcard) {
  RouterTrie trie = RouterTrie('/');
  trie.insert("/test/*", Route());

  std::vector<Route> routes = trie.search("/test/123", GET);
  ASSERT_EQ(routes.size(), 1);
  routes = trie.search("/test/123/456", GET);
  ASSERT_EQ(routes.size(), 1);
}

TEST(RouterTrie, Has) {
  RouterTrie trie = RouterTrie('/');
  Route route = Route();
  route.path = "/test";
  route.method = HTTPMethod::GET;
  route.callback = [](HTTPRequest &request, HTTPResponse &response) {};
  trie.insert(route.path, route);
  ASSERT_TRUE(trie.has(route.path));
  ASSERT_FALSE(trie.has("/test/123"));
}

}