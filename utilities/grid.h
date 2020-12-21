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
#include <array>
#include <functional>


/**
 * @brief enumeration of edge indices
*/
enum Edges
{
   top,
   right,
   bottom,
   left,
   total_edges
};


template <typename T>
class Grid
{
public:
   std::array<std::function<void( void )>, 7> transformations{ std::bind( &Grid:: rotate, this), std::bind( &Grid::rotate, this ),
                                                               std::bind( &Grid::rotate, this ), std::bind( &Grid::reflect, this ), 
                                                               std::bind( &Grid::rotate, this ), std::bind( &Grid::rotate, this ), 
                                                               std::bind( &Grid::rotate, this ) };


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
   std::vector<std::vector<T>> get_edges( )
   {
      std::vector<std::vector<T>> edges;  //!< container to store all the edges

      /* get the two vertical edges */
      std::vector<T> left_edge(this->y_size);
      std::vector<T> right_edge( this->y_size );
      std::transform(tiles.begin(), tiles.end(), left_edge.begin(), [](std::vector<T> row){ return row.front();} );
      std::transform( tiles.begin(), tiles.end(), right_edge.begin(), []( std::vector<T> row ) { return row.back(); } );

      /* put all edges into the container in order */
      edges.push_back( tiles.front() ); //!< top edge
      edges.push_back( right_edge );    //!< right edge
      edges.push_back( tiles.back() );  //!< bottom edge
      edges.push_back( left_edge );     //!< left edge
      return edges;
   }

   /**
    * @brief apply a rotation to the tiles (default left)
   */
   void rotate()
   {
      std::vector<std::vector<T>> copy = tiles;
      for (int i = 0; i < x_size; i++)
      {
         for (int j = 0; j < y_size; j++)
         {
            tiles[j][i] = copy[i][j];
         }
      }
   }

   /**
    * @brief apply a reflection to the data (default Y)
    * @tparam T 
   */
   void reflect()
   {
      std::for_each(tiles.begin(), tiles.end(), 
         [](std::vector<T>& row){std::reverse(row.begin(), row.end());} );
   }



private:
   int id{0};
   size_t x_size{0};
   size_t y_size{0};
   std::vector<std::vector<T>> tiles;
   


};