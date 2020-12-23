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
#include <array>
#include <iostream>
#include <memory>
#include <numeric>
#include <unordered_map>
#include <vector>

/********************************** Types *******************************************/
/**
 * @brief single hashed list node element
 * @tparam T template type parameter
*/
template <typename T>
struct Node {
    T data;
    Node *next;
};

/**
 * @brief circular linked list hashed data structure. This provides fast access,
 *        fast insertion, and fast deletion everywhere in the list, and also maintains
 *        the circular ordering of elements. It is obviously less than memory efficient.
 * @note  as this data structure is designed purely with the AoC 2020 day 23 solution in mind,
 *        there are a few caveats that make this not quite as general as it could be (but more efficient
 *        for the task at hand):
 *          1. It is implicitly assumed that the list only contains unique items
 *          2. the maximum size of the container is constant. Nodes can only be added if they are first
 *             removed. This basically means it is just a re-ordering
 *          
 * @tparam T templated type for the container (not strictly required, but good practice anyhow)
*/
template <typename T>
class CircularList {
  public:
    /**
     * @brief create a new circular hashed list container
     * @param data vector of input data to store in the list
    */
    CircularList( std::vector<T> data )
        : nodes( data.size( ) )
        , size( data.size( ) )
        , minimum( *std::min_element( data.cbegin( ), data.cend( ) ) )
        , maximum( *std::max_element( data.cbegin( ), data.cend( ) ) ) {

        /* setup the circular linked list */
        for ( size_t i = 0; i < size; i++ ) {
            nodes[i].data = data[i];
            nodes[i].next = &nodes[( i + 1LL ) % size];
        }

        /* initialize the hash table */
        for ( size_t i = 0; i < size; i++ ) {
            hash_table[nodes[i].data] = &nodes[i];
        }
    }

    /**
     * @brief remove a set of nodes after a specific element in the list
     * @param element the element to remove nodes after
     * @param count how many elements to pop
     * @return vector of popped nodes
    */
    std::vector<T> remove_after_element( T element, int count ) {
        Node<T> *node = hash_table[element];
        std::vector<T> popped_entries( count );
        for ( int i = 0; i < count; i++ ) {
            popped_entries[i] = node->next->data;
            node->next = node->next->next;
        }
        return popped_entries;
    }

    /**
     * @brief insert a sequences of nodes after a particular element in the collection
     * @param element the element to insert after
     * @param nodes list of nodes
    */
    void insert_after_element( T element, std::vector<T> entries ) {
        Node<T> *node = hash_table[element];
        T final_element = node->next->data;
        for ( auto &entry : entries ) {
            node->next = hash_table[entry];
            node = node->next;
        }
        node->next = hash_table[final_element];
    }

    /**
     * @brief get the next element from an element
     * @param element the element value
     * @return next element
    */
    T get_next( T element ) {
        Node<T> *node = hash_table[element];
        return node->next->data;
    }

    /**
     * @brief getter to get the min item in the list     
    */
    T get_minimum( void ) {
        return minimum;
    }

    /**
     * @brief getter to get the max item in the list     
    */
    T get_maximum( void ) {
        return maximum;
    }

    /**
     * @brief debug function to print the list ordering from an element
     * @param element the element to print from
    */
    void print_from_element( T element ) {
        Node<T> *node = hash_table[element];
        T start_value = node->data;
        std::cout << start_value << " ";

        T data = node->next->data;
        while ( data != start_value ) {
            std::cout << data << " ";
            node = node->next;
            data = node->next->data;
        }
        std::cout << "\n";
    }

  private:
    std::unordered_map<T, Node<T> *> hash_table;
    std::vector<Node<T>> nodes;
    size_t size;
    T maximum;
    T minimum;
};
