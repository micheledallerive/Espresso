//
// Created by michele on 15.01.23.
//

#include <gtest/gtest.h>
#include "lib/trie/RouterTrieNode.h"

namespace Espresso {

TEST(RouterTrieNode, Constructor) {
  RouterTrieNode node;
  ASSERT_EQ(node.key, "");
  ASSERT_EQ(node.routes.size(), 0);
  ASSERT_EQ(node.children.size(), 0);
  ASSERT_EQ(node.parent, nullptr);
  ASSERT_EQ(node.depth, 0);
}

TEST(RouterTrieNode, ConstructorWithDepth) {
  RouterTrieNode node = RouterTrieNode(1);
  ASSERT_EQ(node.key, "");
  ASSERT_EQ(node.routes.size(), 0);
  ASSERT_EQ(node.children.size(), 0);
  ASSERT_EQ(node.parent, nullptr);
  ASSERT_EQ(node.depth, 1);
}

TEST(RouterTrieNode, IsLeaf) {
  RouterTrieNode node = RouterTrieNode(1);
  ASSERT_FALSE(node.isLeaf());
}

TEST(RouterTrieNode, IsRoot) {
  RouterTrieNode node = RouterTrieNode(0);
  ASSERT_TRUE(node.isRoot());
}

}