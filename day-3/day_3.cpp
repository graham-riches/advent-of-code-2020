/*! \file day_3.cpp
*
*  \brief day-3 advent of code problem
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ostream>
#include <regex>
#include <string>
#include <vector>

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/


/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/
int count_on_path( std::vector<std::vector<bool>> map, int over, int down );

/****************************** Functions Definition ***********************************/
/**
 * @brief main application entry point
 * @param argc number of arguments
 * @param argv pointer to array of inputs
 * @return integer return code
*/
int main( int argc, char *argv[] )
{
   std::vector<std::vector<bool>> map;
   std::ifstream tree_map{ std::string{ argv[1] } };
   std::string line;
   while ( std::getline( tree_map, line ) )
   {
      std::vector<bool> trees;
      std::transform( line.cbegin( ), line.cend( ), std::back_inserter(trees), []( char x ) { return ( x == '#' ); } );      
      map.push_back( trees );
   }

   /* part 1 solution */
   auto count = count_on_path( map, 3, 1 );
   std::cout << "hit " << count << " trees" << std::endl;

   /* part 2 solution */
   std::vector<std::pair<int, int>> paths{ { 1, 1 }, { 3, 1 }, { 5, 1 }, { 7, 1 }, { 1, 2 } };
   std::vector<int> trees_per_path( paths.size( ) );

   int trees_multiple = std::transform_reduce(
      paths.cbegin( ), paths.cend( ), 1L, std::multiplies<long int>( ), [map]( std::pair<int, int> p ) { return count_on_path( map, p.first, p.second ); } );
   
   std::cout << "Trees encountered multiple " << trees_multiple;
   
   
   return 0;
}

/**
 * @brief count the number of trees encountered on a path through a forest
 * @param map 2d vector of trees
 * @param over number of locations to move over per iteration
 * @param down how many rows to move down per iteration
 * @return 
*/
int count_on_path(std::vector<std::vector<bool>> map, int over, int down ) 
{   
   int count = std::transform_reduce( map.cbegin( ), map.cend( ), 0L, 
      std::plus<>( ), 
      [line_count = 0, row = 0, col = 0, over, down]( std::vector<bool> line ) mutable 
      {   
         bool result = false;
         if (  line_count % down == 0 )
         {
            result = line[col % line.size( )];
            col += over;
         }         
         line_count += 1;         
         return result;
      }
      );      
   return count;
}