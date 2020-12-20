/*! \file grid.h
*
*  \brief grid data structure for storing 2 dimensional data
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include <algorithm>
#include <numeric>
#include <vector>


enum class Rotation : unsigned
{
   degrees_90,
   degrees_180,
   degrees_270,
};


/**
 * @brief enumeration of axes to handle mirroring operations
*/
enum class Axis : unsigned
{
   x_axis,
   y_axis,
};


template <typename T>
class Grid
{
public:
   /**
    * @brief construct a grid object with a specific size
    * @param id identifier for the grid object
    * @param x_dimension
    * @param y_dimension
   */
   Grid( int id, size_t x_dimension, size_t y_dimension )
      : id(id), tiles( x_dimension, std::vector<T>( y_dimension ) ),
      x_size(x_dimension), y_size(y_dimension)
   {}

   /**
    * @brief construct a grid from a vector of vectors
    * @param id the identifier for the grid object
    * @param grid the input grid
   */
   Grid( int id, std::vector<std::vector<T>> grid )
      : id(id)
   {
      tiles = std::move(grid);
      y_size = tiles.size();
      x_size = tiles[0].size();      
   }

   /**
    * @brief default constructor    
   */
   Grid( void ) {};

   /**
    * @brief get the id of the object
    * @return 
   */
   int get_id() const
   {
      return id;
   }

   /**
    * @brief get the dimensions of the object
    * @return pair of sizes {x,y}
   */
   std::pair<size_t, size_t> get_dimensions() const
   {
      return {x_size, y_size};
   }

   /**
    * @brief return a vector of all edge vectors
    * @return vector of vector of edges
   */
   std::vector<std::vector<T>> get_edges()
   {
      std::vector<std::vector<T>> edges;  //!< container to store all the edges

      /* get the two vertical edges */
      std::vector<T> left_edge(this->y_size);
      std::vector<T> right_edge( this->y_size );
      std::transform(tiles.begin(), tiles.end(), left_edge.begin(), [](std::vector<T> row){ return row.front();} );
      std::transform( tiles.begin(), tiles.end(), right_edge.begin(), []( std::vector<T> row ) { return row.back(); } );

      /* put all edges into the container in order */
      edges.push_back( tiles[0] );     //!< top edge
      edges.push_back( right_edge );   //!< right edge
      edges.push_back( tiles.back() ); //!< bottom edge
      edges.push_back( left_edge );    //!< left edge
      return edges;
   }



private:
   int id{0};
   size_t x_size{0};
   size_t y_size{0};
   std::vector<std::vector<T>> tiles;

};