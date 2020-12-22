/*! \file reconstruct_image.h
*
*  \brief contains utilities for reconstructing the day-20 image puzzle
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include "grid.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <vector>

/********************************** Function Definitions *******************************************/

/********************************** Types *******************************************/
/**
 * @brief enumeration of how many matches a given tile piece will have
 */
enum class TileType : unsigned {
    corner = 2,
    edge = 3,
    interior = 4,
};

/**
 * @brief structure to contain match information
 */
struct Match {
    Match( ){ };
    Match( int id_a, int id_b )
        : id_a( id_a )
        , id_b( id_b ){ };
    Match( int id_a, int id_b, bool match, Edges a, Edges b )
        : id_a( id_a )
        , id_b( id_b )
        , match( match )
        , a_edge( a )
        , b_edge( b ){ };

    /**
    * @brief helper method to get the opposing match id given a tile id
    * @param id the id to get the match for
    * @return opposing tile id
    */
    int get_match_partner( int id ) {
        return ( id == id_a ) ? id_b : id_a;
    }

    int id_a{ 0 };
    int id_b{ 0 };
    bool match{ false };
    bool reverse{ false };
    Edges a_edge{ Edges::top };
    Edges b_edge{ Edges::top };
};

/**
 * @brief structure to store the constructed image in
*/
class ConstructedImage {
  public:
    ConstructedImage( size_t x, size_t y, std::vector<Match> matches, std::map<int, int> types )
        : x_size( x )
        , y_size( y )
        , image( x, std::vector<int>( y ) )
        , matches( matches )
        , tile_type( types ){ };

    /**
    * @brief place all the tile boundaries by marching around the perimeter. The logic is a bit nasty to handle this
    *        and could probably be simplified somewhat. That is currently future optimization as just getting it 
    *        working is the main priority at the moment.
    * @param first_corner_id id of the first corner to place
    */
    void place_edges_and_corners( int first_corner_id ) {
        /* Place the initial tile in the top left corner and store it in the working set */
        stored_tiles.insert( first_corner_id );
        image[0][0] = first_corner_id;

        /* create a map between the traversal direction and which edge we need to match */
        std::map<Edges, Edges> direction_to_match_edge{
            { Edges::top, Edges::left }, { Edges::right, Edges::top }, { Edges::bottom, Edges::right }, { Edges::left, Edges::bottom } };

        /* initialize the iterator locations, and some other control variables */
        int row{ 0 };
        int column{ 0 };
        int current_tile_id{ first_corner_id };
        int last_tile_id{ first_corner_id };
        Edges current_edge{ Edges::top };

        /* fairly heinous while loop to place all the tiles */
        while ( true ) {
            std::vector<Match> children = get_children( current_tile_id );
            std::vector<Match> edges;
            std::vector<Match> corners;

            std::copy_if( children.begin( ), children.end( ), std::back_inserter( edges ), [this, current_tile_id]( Match match ) {
                return tile_type.at( match.get_match_partner( current_tile_id ) ) == static_cast<int>( TileType::edge );
            } );

            std::copy_if( children.begin( ), children.end( ), std::back_inserter( corners ), [this, current_tile_id]( Match match ) {
                return tile_type.at( match.get_match_partner( current_tile_id ) ) == static_cast<int>( TileType::corner );
            } );

            current_tile_id = get_next_id( current_tile_id, edges, corners );

            /* check for completion */
            if ( ( current_tile_id == 0 ) || ( stored_tiles.contains( current_tile_id ) ) ) {
                break;
            }

            /* add the current id to the appropriate spot in the grid */
            switch ( current_edge ) {
                case Edges::top:
                    image[row][++column] = current_tile_id;
                    break;
                case Edges::right:
                    image[++row][column] = current_tile_id;
                    break;
                case Edges::bottom:
                    image[row][--column] = current_tile_id;
                    break;
                case Edges::left:
                    image[--row][column] = current_tile_id;
                    break;
                default:
                    break;
            }

            /* turn the corner and properly rotate the tile if the last tile placed was a corner */
            if ( static_cast<TileType>( tile_type.at( current_tile_id ) ) == TileType::corner ) {
                current_edge = static_cast<Edges>( static_cast<int>( current_edge ) + 1 );
            }

            /* insert the value into the set */
            stored_tiles.insert( current_tile_id );
            last_tile_id = current_tile_id;
        }
    }

    /**
    * @brief place all the interior pieces
    * @note this should always start at [1, 1]
    */
    void place_interior_pieces( void ) {
        /* get all tiles that are not currently in the set */
        std::vector<int> interior_nodes;
        std::transform( tile_type.cbegin( ), tile_type.cend( ), std::back_inserter( interior_nodes ), []( auto map_pair ) {
            return ( static_cast<TileType>( map_pair.second ) == TileType::interior ) ? map_pair.first : 0;
        } );
        interior_nodes.erase( std::remove( interior_nodes.begin( ), interior_nodes.end( ), 0 ), interior_nodes.end( ) );

        for ( int row = 1; row < x_size - 1; row++ ) {
            for ( int column = 1; column < y_size - 1; column++ ) {
                /* look up where the tiles fits : match upper and left neighbour */
                auto left_node_children = get_children( image[row][column - 1LL] );
                auto upper_node_children = get_children( image[row - 1LL][column] );

                /* store all the possible neighbours in a unique set */
                std::set<int> nodes_matching_left = get_child_set( left_node_children );
                std::set<int> nodes_matching_upper = get_child_set( upper_node_children );

                /* the intersection of the two matching sets contains the node we want  */
                std::vector<int> intersection;
                std::set_intersection( nodes_matching_left.begin( ),
                                       nodes_matching_left.end( ),
                                       nodes_matching_upper.begin( ),
                                       nodes_matching_upper.end( ),
                                       std::back_inserter( intersection ) );

                /* the actual node is the intersection of the match set with the interior nodes */
                std::vector<int> match_set;
                std::set_intersection(
                    intersection.begin( ), intersection.end( ), interior_nodes.begin( ), interior_nodes.end( ), std::back_inserter( match_set ) );

                image[row][column] = match_set[0];
                stored_tiles.insert( match_set[0] );
                interior_nodes.erase( std::remove( interior_nodes.begin( ), interior_nodes.end( ), match_set[0] ), interior_nodes.end( ) );
            }
        }
    }

    /**
    * @brief Get the neighbour indices of a location
    *
    * @param row row index
    * @param column index
    * @retval std::vector<std::pair<int, int>>
    */
    std::vector<std::pair<int, int>> get_neighbours( int row, int column ) {
        /* handle the edge and corner cases */
        int row_start = ( row == 0 ) ? 0 : row - 1;
        int row_end = ( row == x_size - 1 ) ? row : row + 1;
        int column_start = ( column == 0 ) ? 0 : column - 1;
        int column_end = ( column == y_size - 1 ) ? column : column + 1;

        /* pack the neighbours into a vector */
        std::vector<std::pair<int, int>> neighbours;
        for ( int i = row_start; i <= row_end; i++ ) {
            for ( int j = column_start; j <= column_end; j++ ) {
                if ( ( i == row ) && ( j != column ) ) {
                    neighbours.push_back( std::pair{ i, j } );
                } else if ( ( j == column ) && ( i != row ) ) {
                    neighbours.push_back( std::pair{ i, j } );
                }
            }
        }
        return neighbours;
    }

    /**
    * @brief get all the matching children to a tile of a specific tile type
    * @param id active ID of the tile
    *
    * @return list of new tiles
    */
    std::vector<Match> get_children( int id ) {
        std::vector<Match> children;
        std::copy_if( matches.begin( ), matches.end( ), std::back_inserter( children ), [this, id]( Match match ) {
            return ( match.id_a == id ) || ( match.id_b == id );
        } );
        return children;
    }

    /**
    * @brief get the next child id from the current edge/corner set
    * @param current_id the current tile id
    * @param edges edges list
    * @param corners list
    * @return 
    */
    int get_next_id( int current_id, std::vector<Match> edges, std::vector<Match> corners ) {
        auto corner_id0 = ( corners.size( ) >= 1 ) ? corners[0].get_match_partner( current_id ) : 0;
        auto corner_id1 = ( corners.size( ) == 2 ) ? corners[1].get_match_partner( current_id ) : 0;
        auto edge_id0 = ( edges.size( ) >= 1 ) ? edges[0].get_match_partner( current_id ) : 0;
        auto edge_id1 = ( edges.size( ) == 2 ) ? edges[1].get_match_partner( current_id ) : 0;
        auto edge_id = ( !stored_tiles.contains( edge_id0 ) ) ? edge_id0 : edge_id1;
        auto corner_id = ( !stored_tiles.contains( corner_id0 ) ) ? corner_id0 : corner_id1;
        current_id = ( edge_id != 0 ) ? edge_id : corner_id;
        return current_id;
    }

    /**
    * @brief get the unique set of child nodes for a set of matches
    * @param matches list of matches
    * @return 
    */
    std::set<int> get_child_set( std::vector<Match> matches ) {
        std::set<int> nodes;
        for ( auto &match : matches ) {
            nodes.insert( match.id_a );
            nodes.insert( match.id_b );
        }
        return nodes;
    }

    /**
    * @brief assemble the final image by rotating every tile in place until the correct combination of tiles is found
    * @param locations locations of each tile in the grid
    * @param tiles raw tile data
    * @return
   */
    std::vector<std::vector<int>> assemble_final_image( std::vector<Grid> &tiles ) {
        /* copy each grid over to its final location */
        std::vector<std::vector<Grid>> grid( x_size, std::vector<Grid>{ y_size } );
        for ( int row = 0; row < x_size; row++ ) {
            for ( int column = 0; column < y_size; column++ ) {
                int id{ image[row][column] };
                auto tile = std::find_if( tiles.cbegin( ), tiles.cend( ), [id]( auto grid ) { return ( grid.get_id( ) == id ); } );
                grid[row][column] = *tile;
            }
        }

        /* rotate all tiles to their correct location */
        for ( int row = 0; row < x_size; row++ ) {
            for ( int column = 0; column < y_size - 1; column++ ) {
                bool done = false;
                for ( int i = 0; i < 7; i++ ) {
                    for ( int j = 0; j < 7; j++ ) {
                        auto parent_edges = grid[row][column].get_edges( );
                        auto child_edges = grid[row][column + 1LL].get_edges( );
                        auto match = std::mismatch( parent_edges[Edges::right].begin( ), parent_edges[Edges::right].end( ), child_edges[Edges::left].begin( ) );
                        if ( match.first == parent_edges[Edges::right].end( ) ) {
                            done = true;
                            break;
                        }
                        grid[row][column + 1LL].next_permutation( );
                    }
                    if ( done )
                        break;

                    grid[row][column + 1LL].next_permutation( );
                    grid[row][column].next_permutation( );
                }
            }
        }

        /* trim the edges of each image: my god this is disgusting */
        auto dim = grid[0][0].get_dimensions( );
        auto size_x = dim.first - 2;
        auto size_y = dim.second - 2;
        std::vector<std::vector<int>> image( size_x * x_size, std::vector<int>( size_y * y_size ) );

        for ( int row = 0; row < x_size; row++ ) {
            for ( int column = 0; column < y_size; column++ ) {
                auto data = grid[row][column].get_interior( );
                auto x = row * size_x;
                auto y = column * size_y;
                for ( int i = 0; i < data.size( ); i++ ) {
                    for ( int j = 0; j < data.size( ); j++ ) {
                        image[x + i][y + j] = data[i][j];
                    }
                }
            }
        }
        return image;
    }

  private:
    size_t x_size;
    size_t y_size;
    std::vector<std::vector<int>> image;
    std::vector<Match> matches;
    std::map<int, int> tile_type;
    std::set<int> stored_tiles;
};
