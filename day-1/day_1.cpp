/*! \file day_1.cpp
*
*  \brief day-1 advent of code problem
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include <ostream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <numeric>
#include <utility>


/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/


/****************************** Functions Prototype ************************************/

template <typename T>
std::vector<std::pair<T, T>> get_pair_combinations( std::vector<T> elements );

template <typename T>
std::vector<std::vector<T>> get_triplet_combinations( std::vector<T> elements );
   /****************************** Functions Definition ***********************************/


/**
 * @brief main application entry point
 * @param argc number of arguments
 * @param argv pointer to array of inputs
 * @return integer return code
*/
int main( int argc, char *argv[] )
{       
   std::vector<int> expenses;

   std::ifstream password_file{ std::string{ argv[1] } };
   std::string line;
   while ( std::getline( password_file, line ) )
   {
      expenses.push_back( std::stoi( line ) );
   }

   auto triplets = get_triplet_combinations( expenses );   
   auto match = std::find_if( triplets.cbegin( ), triplets.cend( ), []( auto p ) { return ( std::accumulate(p.begin(), p.end(), 0L) == 2020 ); } );   
   int multiple = std::accumulate( match->cbegin(), match->cend(), 1, std::multiplies<int>( ) );
   std::cout << "match multiplication total value " << multiple << std::endl;

   return 0;   
}


/**
 * @brief get all pair combinations from a vector
 * @details this returns all possible permutations of two elements in the list excluding
 *          pairs of the same item. Note, order does not matter (i.e. AB is considered equal to BA)
 * @tparam T the type of the pair
 * @param elements range of elements
 * @return vector of pairs
*/
template <typename T>
std::vector<std::pair<T, T>> get_pair_combinations( std::vector<T> elements )
{
   std::vector<std::pair<T, T>> pairs;
   for ( int i = 0; i < elements.size(); i++ )
   {
      for ( int j = i+1; j < elements.size(); j++ )
      {
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
std::vector<std::vector<T>> get_triplet_combinations(std::vector<T> elements)
{
   std::vector<std::vector<T>> triplets;
   for ( int i = 0; i < elements.size( ); i++ )
   {
      for ( int j = i + 1; j < elements.size( ); j++ )
      {
         for ( int k = j + 1; k < elements.size(); k++ )
         {
            triplets.push_back( { elements[i], elements[j], elements[k] } );
         }         
      }
   }
   return triplets;
}