/*! \file grid_tests.cpp
*
*  \brief tests for grid data structure
* 
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include <gtest/gtest.h>
#include <iostream>
#include <algorithm>
#include "grid.h"


TEST( grid_tests, test_constructing_grid )
{
   Grid<int> test_grid(69, 5, 5);   
   auto size = test_grid.get_dimensions();
   EXPECT_EQ(5, size.first);
   EXPECT_EQ(5, size.second);
   EXPECT_EQ(69, test_grid.get_id());
}


TEST( grid_tests, test_constructing_from_vector )
{
   std::vector<int> row{1,2,3,4,5};
   std::vector<std::vector<int>> grid{row, row, row, row, row};
   Grid test_grid{69, grid};
   auto size = test_grid.get_dimensions();
   EXPECT_EQ( 5, size.first );
   EXPECT_EQ( 5, size.second );
   EXPECT_EQ( 69, test_grid.get_id() );
}

TEST( grid_tests, test_getting_edges )
{
   std::vector<int> row{ 1,2,3,4,5 };
   std::vector<std::vector<int>> grid{ row, row, row, row, row };
   Grid test_grid{69, grid};
   auto edges = test_grid.get_edges();
   EXPECT_EQ(4, edges.size());

   std::vector<int> top_bottom{1, 2, 3, 4, 5};
   std::vector<int> left{1, 1, 1, 1, 1};
   std::vector<int> right{5, 5, 5, 5, 5};

   /* check each set of edges to make sure they are valid */
   auto top_match = std::mismatch( top_bottom.cbegin(), top_bottom.cend(), edges[0].cbegin() );
   auto bottom_match = std::mismatch( top_bottom.cbegin(), top_bottom.cend(), edges[2].cbegin() );
   auto left_match = std::mismatch( left.cbegin(), left.cend(), edges[3].cbegin() );
   auto right_match = std::mismatch( right.cbegin(), right.cend(), edges[1].cbegin() );   
   
   EXPECT_EQ( top_bottom.cend(), top_match.first );
   EXPECT_EQ( top_bottom.cend(), bottom_match.first );
   EXPECT_EQ( left.cend(), left_match.first );
   EXPECT_EQ( right.cend(), right_match.first );   
}

