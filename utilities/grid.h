/*! \file grid.h
*
*  \brief grid data structure for storing 2 dimensional data
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include <array>
#include <vector>


constexpr int total_permutations = 7;

/**
 * @brief enumeration of edge indices
 */
enum Edges { top, right, bottom, left, total_edges };



template <typename T>
class Grid {
  public:        
    Grid( int id, size_t x_dimension, size_t y_dimension )
    Grid( int id, std::vector<std::vector<T>> grid );
    Grid( void ){ };
    int get_id( ) const;    
    std::pair<size_t, size_t> get_dimensions( ) const;
    std::vector<std::vector<T>> get_edges( );    
    static void rotate( Grid &grid );
    static void reflect( Grid &grid );

  private:
    int id{ 0 };
    size_t x_size{ 0 };
    size_t y_size{ 0 };
    std::vector<std::vector<T>> tiles;
};
