/*! \file graph_tests.cpp
*
*  \brief tests for graph data structure 
* 
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include <gtest/gtest.h>
#include <iostream>
#include <algorithm>
#include "graph.h"

using namespace graph;


/**
 * @brief test class that mimics a more complex graph structure
*/
class TestComplexGraphSetup : public ::testing::Test
{
protected:
   void SetUp() override
   {
      this->graph = Graph<char>{6};
      this->graph.nodes[0] = Node<char>{ 0 };
      this->graph.nodes[1] = Node<char>{ 1 };
      this->graph.nodes[2] = Node<char>{ 2 };
      this->graph.nodes[3] = Node<char>{ 3 };
      this->graph.nodes[4] = Node<char>{ 4, 'a' };
      this->graph.nodes[5] = Node<char>{ 5, 'b' };

      graph.assign_child_nodes( 0, ChildNodes::child_a, std::vector<int64>{4, 1, 5} );
      graph.assign_child_nodes( 1, ChildNodes::child_a, std::vector<int64>{2, 3} );
      graph.assign_child_nodes( 1, ChildNodes::child_b, std::vector<int64>{3, 2} );
      graph.assign_child_nodes( 2, ChildNodes::child_a, std::vector<int64>{4, 4} );
      graph.assign_child_nodes( 2, ChildNodes::child_b, std::vector<int64>{5, 5} );
      graph.assign_child_nodes( 3, ChildNodes::child_a, std::vector<int64>{4, 5} );
      graph.assign_child_nodes( 3, ChildNodes::child_b, std::vector<int64>{5, 4} );
   }

   void TearDown() override
   {

   }

public:
   Graph<char> graph;


};



TEST( graph_tests, test_creating_graph )
{   
   Graph<int> graph{4};
   EXPECT_EQ(4, graph.nodes.size());
}

TEST( graph_tests, test_create_node_with_data )
{
   Node<char> node{1, 'C'};
   char data = *node.data.get();
   EXPECT_EQ( 'C', data );
}

TEST( graph_tests, test_creating_node_default )
{
   Node<int> node;
   EXPECT_EQ( 0, node.key );
   EXPECT_EQ( nullptr, node.data );
   EXPECT_EQ( 2, node.children.size() );
   EXPECT_EQ( 0, node.children[0].size() );
   EXPECT_EQ( 0, node.children[1].size() );
}

TEST( graph_tests, test_creating_node_with_only_key )
{
   Node<int> node{15};
   EXPECT_EQ( 15, node.key );
   EXPECT_EQ( nullptr, node.data );
   EXPECT_EQ( 0, node.children[0].size() );
   EXPECT_EQ( 0, node.children[1].size() );
}

TEST( graph_tests, test_move_construtor )
{
   Node<int> node{Node<int>{15, 0x69420LL}};
   EXPECT_EQ( 15, node.key );
   EXPECT_EQ( 0x69420LL, *node.data.get());
}

TEST( graph_tests, test_node_move_assignment_operator )
{
   Node<std::string> node;
   node = Node<std::string>{15, std::string{"test_string"} };
   EXPECT_EQ( std::string{"test_string"}, *node.data.get() );
   EXPECT_EQ(15, node.key);
}

TEST( graph_tests, test_get_node_pointer_by_key )
{
   Graph<std::string> graph(2);
   graph.nodes[0] = Node<std::string>{69, std::string{"node one"}};
   graph.nodes[1] = Node<std::string>{420, std::string{"node two"}};
   auto ptr = graph.get_node_by_key(69);
   ASSERT_TRUE( ptr != nullptr );
   EXPECT_EQ( 69, ptr->key);
}

TEST( graph_tests, test_adding_children )
{
   Graph<std::string> graph( 2 );
   graph.nodes[0] = Node<std::string>{ 69, std::string{"node one"} };
   graph.nodes[1] = Node<std::string>{ 420, std::string{"node two"} };

   /* make each node point at each other */
   graph.nodes[0].append_child( ChildNodes::child_a, graph.get_node_by_key(420) );
   graph.nodes[1].append_child( ChildNodes::child_b, graph.get_node_by_key(69) );

   EXPECT_EQ( 1, graph.nodes[0].children[0].size() );
   EXPECT_EQ( 1, graph.nodes[1].children[1].size() );
   EXPECT_EQ( &graph.nodes[0], graph.nodes[1].children[1][0] );
   EXPECT_EQ( &graph.nodes[1], graph.nodes[0].children[0][0] );
}

TEST_F( TestComplexGraphSetup, test_appending_children_from_vector )
{        
   auto nodes = graph.nodes[0].get_child_nodes(ChildNodes::child_a);

   EXPECT_EQ( 3, nodes.size() );
   EXPECT_EQ( &graph.nodes[4], nodes[0] );
   EXPECT_EQ( &graph.nodes[1], nodes[1] );
   EXPECT_EQ( &graph.nodes[5], nodes[2] );
}

TEST_F( TestComplexGraphSetup, DISABLED_test_getting_combinations_from_one_level_up )
{
   auto test = graph.get_child_combinations(2);
   EXPECT_EQ(2, test.size());
}

TEST_F( TestComplexGraphSetup, DISABLED_test_getting_combinations_from_two_levels_up )
{
   auto test = graph.get_child_combinations( 1 );
   EXPECT_EQ( 8, test.size() );
}