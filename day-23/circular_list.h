/*! \file circular_list.h
*
*  \brief day-23 advent of code problem circular list data structure
* 
*
*  \author Graham Riches
*  \details this is a circular linked list essentially
*/

#pragma once

/********************************** Includes *******************************************/
#include <algorithm>
#include <memory>
#include <numeric>
#include <vector>

template <typename T>
struct Node {
    T data;
    Node *next;
};


template <typename T>
class CircularList {
  public:
    CircularList( std::vector<T> data ) {
        /* create the linked list */
        size = data.size( );
        nodes = std::make_unique<Node<T>[]>( size );
        nodes[0].next = &nodes[1];
        for ( size_t i = 0; i < size - 1; i++ ) {
            nodes[i].data = data[i];
            nodes[i].next = &nodes[i + 1LL];
        }
        nodes[size].next = &nodes[0];

        /* set the start node */
        active_node = &nodes[0];
    }

  private:
    std::unique_ptr<Node<T>[]> nodes;
    Node<T> *active_node;
    size_t size;
};
