/*! \file combinations.h
*
*  \brief functions to get unique combinations of things
*
*
*  \author Graham Riches
*/

#pragma once

#include <vector>

/**
 * @brief get all pair combinations from a vector
 * @details this returns all possible permutations of two elements in the list excluding
 *          pairs of the same item. Note, order does not matter (i.e. AB is considered equal to BA)
 * @tparam T the type of the pair
 * @param elements range of elements
 * @return vector of pairs
*/
template <typename T>
std::vector<std::pair<T, T>> get_pair_combinations( std::vector<T> elements ) {
    std::vector<std::pair<T, T>> pairs;
    for ( int i = 0; i < elements.size( ); i++ ) {
        for ( int j = i + 1; j < elements.size( ); j++ ) {
            pairs.push_back( std::pair<T, T>{ elements[i], elements[j] } );
        }
    }
    return pairs;
}

/**
 * @brief get all triplet combinations from a vector
 * @details this returns all possible permutations of three elements in the list excluding
 *          the same item. Note, order does not matter (i.e. ABC is considered equal to BCA)
 * @tparam T the type of the pair
 * @param elements range of elements
 * @return vector of pairs
*/
template <typename T>
std::vector<std::vector<T>> get_triplet_combinations( std::vector<T> elements ) {
    std::vector<std::vector<T>> triplets;
    for ( int i = 0; i < elements.size( ); i++ ) {
        for ( int j = i + 1; j < elements.size( ); j++ ) {
            for ( int k = j + 1; k < elements.size( ); k++ ) {
                triplets.push_back( { elements[i], elements[j], elements[k] } );
            }
        }
    }
    return triplets;
}