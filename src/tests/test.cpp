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

TEST(GraphTests, Init_with_V) {
  param_vc::Graph g = param_vc::Graph(3);
  ASSERT_EQ(g.V,3);
  ASSERT_EQ(g.E,0);
  ASSERT_EQ(g.adj_lists.size(),3);
  ASSERT_EQ(g.exists.size(),3);
}

TEST(GraphTests, Manipulations) {
  int size = 5;
  int ecount = 0;
  param_vc::Graph g = param_vc::Graph(size);
  ASSERT_EQ(g.V,size);
  ASSERT_EQ(g.E,ecount);
  ASSERT_EQ(g.adj_lists.size(),size);
  ASSERT_EQ(g.exists.size(),size);

  g.add_edge(1,2);
  ASSERT_EQ(g.adj_lists[1].size(),1);
  g.add_edge(2,3);
  g.add_edge(3,3);
  g.add_edge(4,5); // shouldnt work
  ASSERT_EQ(g.adj_lists[4].size(),0);
  g.add_edge(2,4);
  g.add_edge(0,2);
  ASSERT_EQ(g.adj_lists[0].size(),1);
  ASSERT_EQ(g.adj_lists[2].size(),4);
  ASSERT_EQ(g.adj_lists[3].size(),1);
  ASSERT_EQ(g.adj_lists[4].size(),1);

  g.remove_vertex(2);
  ASSERT_EQ(g.exists[2],false);
  ASSERT_EQ(g.adj_lists[0].size(),0);
  ASSERT_EQ(g.adj_lists[1].size(),0);
  ASSERT_EQ(g.adj_lists[2].size(),4); // we shouldnt touch the list of a dead node
  ASSERT_EQ(g.adj_lists[3].size(),0);
  ASSERT_EQ(g.adj_lists[4].size(),0);
}

TEST(VC_Tests, Kernelization) {
  int size = 10;
  int param = 4;
  int ecount = 0;
  param_vc::Graph g = param_vc::Graph(size);
  g.add_edge(1,2);
  g.add_edge(2,3);
  g.add_edge(3,3);
  g.add_edge(4,5);
  g.add_edge(2,4);
  g.add_edge(0,2);

  param_vc::ParameterizedVertexCover vc = param_vc::ParameterizedVertexCover(g,param);

  //TODO

}

TEST(VC_Tests, Empty_case) {
  param_vc::Graph g = param_vc::Graph();
  param_vc::ParameterizedVertexCover vc = param_vc::ParameterizedVertexCover(g,1);
  ASSERT_LE(vc.has_vertex_cover(), 0);
}