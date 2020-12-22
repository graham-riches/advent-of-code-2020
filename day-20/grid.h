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


/**
 * @brief enumeration of edge indices
 */
enum Edges { top, right, bottom, left, total_edges };

class Grid {
  public:
    Grid( int id, size_t x_dimension, size_t y_dimension );
    Grid( int id, std::vector<std::vector<int>> grid );
    Grid( void ){ };
    int get_id( ) const;
    std::pair<size_t, size_t> get_dimensions( ) const;
    std::vector<std::vector<int>> get_edges( );
    std::vector<std::vector<int>> get_interior( );
    void next_permutation( );    

  private:
    void reflect( void );
    void rotate( void );

    int permutation{ 0 };
    int id{ 0 };
    size_t x_size{ 0 };
    size_t y_size{ 0 };
    std::vector<std::vector<int>> tiles;

    typedef void ( Grid::*Permutation )( void );
    Permutation permutations[8] = {
        &Grid::rotate,
        &Grid::rotate,
        &Grid::rotate,
        &Grid::reflect,
        &Grid::rotate,
        &Grid::rotate,
        &Grid::rotate,
        &Grid::rotate,
    };
};
