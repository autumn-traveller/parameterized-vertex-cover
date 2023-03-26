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

TEST(GraphTests, InitWithV) {
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

TEST(VC_Tests, EmptyCase) {
  param_vc::Graph g = param_vc::Graph();
  param_vc::ParameterizedVertexCover vc = param_vc::ParameterizedVertexCover(g,1);
  ASSERT_TRUE(vc.has_vertex_cover());
}

TEST(VC_Tests, KernelizationAndRecovery) {
  int size = 5;
  int param = 1;
  int ecount = 0;
  param_vc::Graph g = param_vc::Graph(size);
  // VC on a star graph accepts k = 1 . our kernelization algorithm should be able to "solve" this, we will test it later on an almost star graph, which would require a vc of 2
  g.add_edge(0,1);
  g.add_edge(0,2);
  g.add_edge(0,3);
  g.add_edge(0,4);

  param_vc::ParameterizedVertexCover vc = param_vc::ParameterizedVertexCover(g,param);

  ASSERT_EQ(vc.done,false);
  vc.kernelize();
  ASSERT_EQ(vc.kprime,0);
  ASSERT_EQ(vc.partial_solution.size(),1);
  ASSERT_EQ(vc.partial_solution[0],0);
  ASSERT_EQ(g.exists[0],false);
  ASSERT_EQ(g.exists[1],false);
  ASSERT_EQ(g.exists[2],false);
  ASSERT_EQ(g.exists[3],false); // they should all have been deleted
  ASSERT_EQ(g.exists[4],false);

  g.recover_deleted_vertices();

  ASSERT_EQ(g.adj_lists[0].size(),4);
  ASSERT_EQ(g.adj_lists[1].size(),1);
  ASSERT_EQ(g.adj_lists[4].size(),1);

  // now we add an edge which makes vc impossible
  vc.kprime = param;
  // vc.partial_solution.clear(); // TODO: does clear delete everything?
  vc.partial_solution.erase( vc.partial_solution.begin() + 0);
  ASSERT_EQ(vc.partial_solution.size(),0);

  g.add_edge(1,2); // now there is no solution

  vc.kernelize();
  ASSERT_EQ(vc.done,true);
  ASSERT_EQ(vc.has_solution,false);
}

TEST(VC_Tests, SolveStarGraphNoKernel) {
  int size = 8;
  int param = 2;
  int ecount = 0;
  param_vc::Graph g = param_vc::Graph(size);
  // almost a star graph
  for(int i = 1; i < size; i++)
    g.add_edge(0,i);
  g.add_edge(6,5);
  g.add_edge(6,7);

  param_vc::ParameterizedVertexCover vc = param_vc::ParameterizedVertexCover(g,param);

  ASSERT_TRUE(vc.branch(0));
  ASSERT_EQ(vc.partial_solution.size(),2);
  ASSERT_EQ(vc.partial_solution[0],0);
  ASSERT_EQ(vc.partial_solution[1],6);
  ASSERT_EQ(vc.done,false);
  ASSERT_EQ(vc.has_solution,true);
}

TEST(VC_Tests, FailStarGraphNoKernel) {
  int size = 8;
  int param = 1;
  int ecount = 0;
  param_vc::Graph g = param_vc::Graph(size);
  // almost a star graph
  for(int i = 1; i < size; i++)
    g.add_edge(0,i);
  g.add_edge(6,5);
  g.add_edge(6,7);

  param_vc::ParameterizedVertexCover vc = param_vc::ParameterizedVertexCover(g,param);

  ASSERT_FALSE(vc.branch(0));
  ASSERT_FALSE(vc.done);
  ASSERT_FALSE(vc.has_solution);
}


TEST(VC_Tests, FullTest1) {
  int size = 8;
  int param = 2;
  int ecount = 0;
  param_vc::Graph g = param_vc::Graph(size);
  // almost a star graph
  for(int i = 1; i < size; i++)
    g.add_edge(0,i);
  g.add_edge(6,5);
  g.add_edge(6,7);

  param_vc::ParameterizedVertexCover vc = param_vc::ParameterizedVertexCover(g,param);

  ASSERT_TRUE(vc.has_vertex_cover());
  ASSERT_EQ(vc.partial_solution.size(),2);
  ASSERT_EQ(vc.partial_solution[0],0);
  ASSERT_EQ(vc.partial_solution[1],6);
  ASSERT_EQ(vc.done,true);
  ASSERT_EQ(vc.has_solution,true);
}

TEST(VC_Tests, FullTest2) {
  int size = 8;
  int param = 1;
  int ecount = 0;
  param_vc::Graph g = param_vc::Graph(size);
  // almost a star graph
  for(int i = 1; i < size; i++)
    g.add_edge(0,i);
  g.add_edge(6,5);
  g.add_edge(6,7);

  param_vc::ParameterizedVertexCover vc = param_vc::ParameterizedVertexCover(g,param);

  ASSERT_FALSE(vc.has_vertex_cover());
  ASSERT_TRUE(vc.done);
  ASSERT_FALSE(vc.has_solution);
}