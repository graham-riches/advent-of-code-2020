/*! \file day_17.cpp
*
*  \brief day-17 advent of code problem
* 
*
*  \author Graham Riches
* 
*  \details this problem is basically a cellular automata base on Conway's game of life,
*           but in three dimensions. The initial starting grid is a single plane in XY.
*/

/********************************** Includes *******************************************/
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <set>
#include "cube.h"
#include "string_utilities.h"



/************************************ Types ********************************************/



/****************************** Functions Definition ***********************************/
/**
 * @brief get the input set of active cubes from the puzzle input
 * @tparam Cube_N cube class type: must have a X, Y, only constructor
 * @param filename the filename to read
 * @return set of unique cubes
 * @note: input data is always in the plane Z
 * 
*/
template <class Cube_N>
std::set<Cube_N> get_input_set(const std::string& filename)
{
   std::vector<std::string> lines = read_file(open_file(filename));
   
   std::set<Cube_N> set;
   for (int row = 0; row < lines.size(); row++)
   {
      for (int column = 0; column < lines[row].size(); column++ )
      {
         if (lines[row][column] == '#')
         {
            set.insert( Cube_N{row, column});
         }
      }      
   }
   return set;
}

/**
 * @brief run the simulation for any cube type
 * @tparam Cube_N class type of the cube
 * @param set initial set of cube data
 * @return new set
*/
template <class Cube_N>
std::set<Cube_N> run_iteration(const std::set<Cube_N>& set)
{
   std::set<Cube_N> new_set;
   std::set<Cube_N> checked_values;
   for (const auto& cube : set)
   {
      auto neighbours = cube.get_neighbours();
      for (const auto& neighbour : neighbours)
      {
         if (!checked_values.contains(neighbour))
         {
            auto bounding_cube = neighbour.get_neighbours();
            auto count = std::count_if(bounding_cube.cbegin(), bounding_cube.cend(),
               [set](const Cube_N c)
               {
                  return set.contains(c);
               }
            );

            if (set.contains(neighbour) && ((count == 2) || (count == 3)))
            {               
               new_set.insert(neighbour);               
            }
            else if ( !set.contains(neighbour) && (count == 3))
            {               
               new_set.insert(neighbour);             
            }
            checked_values.insert(neighbour);
         }         
      }
   }

   return new_set;
}


/**
 * @brief main application entry point
 * @param argc number of arguments
 * @param argv pointer to array of inputs
 * @return integer return code
 * @note command line argument is the path to the directory containing the data
*/
int main( int argc, char *argv[] )
{
   /*------------------------------ Part 1 Solution ------------------------------*/
   std::set<Cube3D> active_set_3D = get_input_set<Cube3D>(std::string{argv[1]});   
   for (int iteration = 0; iteration < 6; iteration++)
   {
      active_set_3D = run_iteration( active_set_3D );
   }
   
   std::cout << "total size: " << active_set_3D.size() << "\n";


   /*------------------------------ Part 2 Solution ------------------------------*/
   std::set<Cube4D> active_set4D = get_input_set<Cube4D>( std::string{ argv[1] } );
   for (int iteration = 0; iteration < 6; iteration++)
   {
      active_set4D = run_iteration( active_set4D );
   }

   std::cout << "total size: " << active_set4D.size() << "\n";


   return 0;
}
