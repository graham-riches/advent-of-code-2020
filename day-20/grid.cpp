/*! \file grid.cpp
*
*  \brief grid data structure for storing 2 dimensional data
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include <algorithm>
#include <array>
#include <functional>
#include <numeric>
#include <vector>
#include "grid.h"


/**
* @brief construct a grid object with a specific size
* @param id identifier for the grid object
* @param x_dimension
* @param y_dimension
*/
Grid::Grid( int id, size_t x_dimension, size_t y_dimension )
    : id( id )
    , tiles( x_dimension, std::vector<int>( y_dimension ) )
    , x_size( x_dimension )
    , y_size( y_dimension ) { }

/**
* @brief construct a grid from a vector of vectors
* @param id the identifier for the grid object
* @param grid the input grid
*/
Grid::Grid( int id, std::vector<std::vector<int>> grid )
    : id( id ) {
    tiles = std::move( grid );
    y_size = tiles.size( );
    x_size = tiles[0].size( );
}

/**
* @brief return a vector of all edge vectors
* @return vector of vector of edges
*/
std::vector<std::vector<int>> Grid::get_edges( ) {
    std::vector<std::vector<int>> edges;  //!< container to store all the edges

    /* get the two vertical edges */
    std::vector<int> left_edge( this->y_size );
    std::vector<int> right_edge( this->y_size );
    std::transform( tiles.begin( ), tiles.end( ), left_edge.begin( ), []( std::vector<int> row ) { return row.front( ); } );
    std::transform( tiles.begin( ), tiles.end( ), right_edge.begin( ), []( std::vector<int> row ) { return row.back( ); } );

    /* put all edges into the container in order */
    edges.push_back( tiles.front( ) );  //!< top edge
    edges.push_back( right_edge );      //!< right edge
    edges.push_back( tiles.back( ) );   //!< bottom edge
    edges.push_back( left_edge );       //!< left edge
    return edges;
}

/**
 * @brief 
 * @return 
*/
std::vector<std::vector<int>> Grid::get_interior() {
    std::vector<std::vector<int>> interior( tiles.size() - 2, std::vector<int>(tiles.size() - 2));
    for ( int i = 1; i < x_size - 1; i++ ) {
        std::copy( tiles[i].begin( ) + 1, tiles[i].end( ) - 1, interior[i - 1LL].begin());
    }
    return interior;
}

/**
* @brief get the dimensions of the object
* @return pair of sizes {x,y}
*/
std::pair<size_t, size_t> Grid::get_dimensions( ) const {
    return { x_size, y_size };
}

/**
* @brief get the id of the object
* @return 
*/
int Grid::get_id( ) const {
    return id;
}

/**
 * @brief get the next tile permutation for a grid object 
*/
void Grid::next_permutation( void ) {
    ( this->*(permutations[permutation++]) )( );
    permutation %= 8;
}


/**
* @brief apply a rotation to a grid
*/
void Grid::rotate( void ) {
    auto copy = tiles;
    for ( int i = 0; i < x_size; i++ ) {
        for ( int j = 0; j < y_size; j++ ) {
            tiles[j][i] = copy[i][j];
        }
    }
}

/**
* @brief apply a reflection to the data (default Y)
* @tparam T 
*/
void Grid::reflect( void ) {
    std::for_each( tiles.begin( ), tiles.end( ), []( std::vector<int> &row ) { std::reverse( row.begin( ), row.end( ) ); } );
}
