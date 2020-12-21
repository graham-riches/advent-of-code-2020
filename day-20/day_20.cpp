/*! \file day_20.cpp
*
*  \brief day-20 advent of code problem
* 
*
*  \author Graham Riches
*  \details sets of 2d array matching. Maybe I'm actually smart enough to do this one :P
*/

/********************************** Includes *******************************************/
#include <algorithm>
#include <memory>
#include <numeric>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <cstdint>
#include <chrono>
#include "string_utilities.h"
#include "grid.h"
#include "combinations.h"
#include "reconstruct_image.h"



/********************************** Functions *******************************************/
/**
 * @brief get a string containing each tiles data in a vector
 * @param filename filename to read data from
 * @return 
*/
std::vector<std::string> get_tile_strings( const std::string& filename )
{
   //!< open the file and read the contents in one shot
   std::ifstream stream{filename};
   std::string all_file_contents{ std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>() };

   //!< split based on empty lines
   std::vector<std::string> tile_data = split(all_file_contents, "\n\n");
   return tile_data;
}


/**
 * @brief convert a tile string into a grid object
 * @param tile_string string containing the tile information
 * @return new grid object
*/
Grid<int> tile_string_to_grid( std::string& tile_string )
{
   std::vector<std::string> lines = split(tile_string, "\n");
   
   /* extract the id from the first line */
   int id = std::stoi(strip(strip(lines[0], "Tile "), ":"));

   /* get the tile values: # = 1, . = 0 */
   std::vector<std::vector<int>> grid(lines.size() - 1, std::vector<int>(lines[1].size()));

   auto string_to_int = [](std::string input)
   {
      std::vector<int> values(input.size());
      std::transform(input.begin(), input.end(), values.begin(), [](char a){ return ( a == '#'); });
      return values;
   };
   std::transform(lines.cbegin() + 1, lines.cend(), grid.begin(), string_to_int );

   return Grid<int>{id, grid};
}


/**
 * @brief count the number of possible edge matches between two grids and return the edge that matches
 * @param a grid A
 * @param b grid B
 * @return Note: two grids can match at most once
 * @note there are only 8 permutations that two grids can match, and these can be found
 *       by just comparing each edge vector to the other edge vector without rotating the
 *       whole grid. In fact, using reverse iterators means that ZERO rotations and flips are 
 *       required, which is super cool.
*/
Match check_all_matching_edges( Grid<int> a, Grid<int> b )
{
   auto a_edges = a.get_edges();
   auto b_edges = b.get_edges();
   Match match{a.get_id(), b.get_id()};

   for ( int a_edge = Edges::top; a_edge < a_edges.size(); a_edge++ )
   {      
      for ( int b_edge = Edges::top; b_edge < b_edges.size(); b_edge++ )
      {
         /* check forward and reverse combinations */
         auto forward_match = std::mismatch( a_edges[a_edge].cbegin(), a_edges[a_edge].cend(), b_edges[b_edge].cbegin() );
         auto reverse_match = std::mismatch( a_edges[a_edge].cbegin(), a_edges[a_edge].cend(), b_edges[b_edge].crbegin() );
         if ((forward_match.first == a_edges[a_edge].cend()) || (reverse_match.first == a_edges[a_edge].cend()))
         {
            match.match = true;
            match.a_edge = static_cast<Edges>(a_edge);
            match.b_edge = static_cast<Edges>(b_edge);       
            match.reverse = (reverse_match.first == a_edges[a_edge].cend());
            goto exit_loops; /* Super Performance Hack: use a goto to break out of the double nesting to shorten cycles */
         }
      }      
   }

   exit_loops:
   return match;
}


/**
 * @brief main application entry point
 * @param argc number of arguments
 * @param argv pointer to array of inputs
 * @return integer return codeout
 * 
 * @note command line argument is the path to the directory containing the data
*/
int main( int64_t argc, char *argv[] )
{  
   /* start the run-time clock */
   auto start = std::chrono::steady_clock::now();

   /* read the data into a vector of grid objects/tiles */
   std::vector<std::string> tile_data = get_tile_strings(std::string{argv[1]});
   std::vector<Grid<int>> tiles(tile_data.size());
   std::transform(tile_data.begin(), tile_data.end(), tiles.begin(), tile_string_to_grid);

   /*------------------------------ Part One Solution ------------------------------*/
   /* get all pair combinations of tiles to check */
   auto tile_pairs = get_pair_combinations(tiles);

   /* count matches between tile combinations and return only matched tiles */
   std::vector<Match> matches(tile_pairs.size());
   std::transform(tile_pairs.cbegin(), tile_pairs.cend(), matches.begin(),
      []( const auto pair )
      {
         auto match_container = check_all_matching_edges( pair.first, pair.second );
         return match_container;
      }
   );

   /* remove any non-matches */
   matches.erase(std::remove_if(matches.begin(), matches.end(), [](auto match){ return ( match.match == false ); } ), matches.end());

   /* count the occurence of each tile in the matched set -> this determines it's possible locations
      2 matches - corner, 3 matches - edge, 4 matches - interior
   */
   std::map<int, int> id_match_map;  //!< create a map of IDs to match counts
   std::for_each(tiles.cbegin(), tiles.cend(), [&id_match_map](auto tile){id_match_map.insert( {tile.get_id(), 0} ); });

   /* tally up the matches for each tile */
   std::for_each(matches.begin(), matches.end(), 
      [&id_match_map](auto match)
      {
         id_match_map[match.id_a]++;
         id_match_map[match.id_b]++;         
      }
   );

   /* multiply the ID's of all corners together */
   auto multiple = std::transform_reduce(id_match_map.cbegin(), id_match_map.cend(), int64_t{1}, std::multiplies<int64_t>(), 
      [](auto map_value)
      {        
         return ( map_value.second == 2 ) ? map_value.first : 1;         
      }
   );

   std::cout << "Multiple of corner Id's is: " << multiple << "\n";



   /*------------------------------ Part Two Solution ------------------------------*/
   /* need to assemble the puzzle from the corners out - this is going to be pretty damn rough to do */
   auto first_corner = std::find_if( id_match_map.cbegin(), id_match_map.cend(), [](auto match_pair){ return (match_pair.second == 2); } );
   size_t dimension{static_cast<size_t>(sqrt(tile_data.size()))};
   ConstructedImage image{ dimension, dimension, matches, id_match_map };
   image.place_edges_and_corners( first_corner->first );
   image.place_interior_pieces();
      
   /* print out the total run time */
   auto end = std::chrono::steady_clock::now();
   std::cout << "Run time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds \n";
   return 0;
}
