#include <gtest/gtest.h>
#include "param_vc.h"

// Demonstrate some basic assertions.

TEST(BasicTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(BasicTest, BasicAssertions2) {
  ASSERT_STREQ("hello", "hello");
  EXPECT_EQ(7 * 6, 42);
}

TEST(GraphTests, Basic_init) {
  param_vc::Graph g = param_vc::Graph();
  param_vc::ParameterizedVertexCover vc = param_vc::ParameterizedVertexCover(g,16);
  ASSERT_EQ(vc.k,16);
}

TEST(VC_Tests, Empty_case) {
  param_vc::Graph g = param_vc::Graph();
  param_vc::ParameterizedVertexCover vc = param_vc::ParameterizedVertexCover(g,1);
  ASSERT_LE(vc.has_vertex_cover(), 0);
}