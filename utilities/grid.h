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

/**
 * @brief enumeration of all possible orientations of a tile
*/
enum class Permutation : unsigned
{
   original,
   rotate_90,
   rotate_180,
   rotate_270,
   mirror_x,
   mirror_x_rotate_90,
   mirror_y,
   mirror_y_rotate_90
};

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
   std::vector<std::vector<T>> get_edges( Permutation permutation )
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

      /* we now have the default edge permutation, the rest are trivial standard algorithm operations */
      switch (permutation)
      {
      case Permutation::original:
         break;

      case Permutation::rotate_90:
         std::rotate(edges.rbegin(), edges.rbegin() + 1, edges.rend());
         break;

      case Permutation::rotate_180:
         std::rotate( edges.rbegin(), edges.rbegin() + 2, edges.rend() );
         break;

      case Permutation::rotate_270:
         std::rotate( edges.rbegin(), edges.rbegin() + 3, edges.rend() );
         break;

      case Permutation::mirror_x:
         /* swap the top and bottom values, and reverse the left and right sides */
         std::iter_swap(edges.begin(), edges.begin() + 2);
         std::reverse(edges[Edges::left].begin(), edges[Edges::left].end());
         std::reverse(edges[Edges::right].begin(), edges[Edges::right].end());
         break;

      case Permutation::mirror_x_rotate_90:
         /* mirror, as above, then rotate */
         std::iter_swap(edges.begin(), edges.begin() + 2);
         std::reverse(edges[Edges::left].begin(), edges[Edges::left].end());
         std::reverse(edges[Edges::right].begin(), edges[Edges::right].end());
         std::rotate( edges.rbegin(), edges.rbegin() + 1, edges.rend() );
         break;

      case Permutation::mirror_y:
         /* swap the left and right sides and reverse the top and bottom */
         std::iter_swap(edges.begin() + 1, edges.begin() + 3);
         std::reverse(edges[Edges::top].begin(), edges[Edges::top].end());
         std::reverse(edges[Edges::bottom].begin(), edges[Edges::bottom].end());
         break;

      case Permutation::mirror_y_rotate_90:
         /* mirror as above, then rotate 90 degrees */
         std::iter_swap(edges.begin() + 1, edges.begin() + 3);
         std::reverse(edges[Edges::top].begin(), edges[Edges::top].end());
         std::reverse(edges[Edges::bottom].begin(), edges[Edges::bottom].end());
         std::rotate( edges.rbegin(), edges.rbegin() + 1, edges.rend() );
         break;

      default:
         break;
      }

      return edges;
   }



private:
   int id{0};
   size_t x_size{0};
   size_t y_size{0};
   std::vector<std::vector<T>> tiles;

};