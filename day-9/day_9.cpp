/*! \file day_7.cpp
*
*  \brief day-7 advent of code problem
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ostream>
#include <regex>
#include <string>
#include <vector>
#include <functional>
#include <set>
#include <map>
#include <cstdint>


/*********************************** Consts ********************************************/

/************************************ Types ********************************************/


/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype **************************************/
/**
 * @brief get all pair combinations from a vector
 * @details this returns all possible permutations of two elements in the list excluding
 *          pairs of the same item. Note, order does not matter (i.e. AB is considered equal to BA)
 * @tparam T the type of the pair
 * @param elements range of elements
 * @return vector of pairs
*/
template <typename T>
std::vector<std::pair<T, T>> get_pair_combinations(std::vector<T> elements)
{
   std::vector<std::pair<T, T>> pairs;
   for (int i = 0; i < elements.size(); i++)
   {
      for (int j = i + 1; j < elements.size(); j++)
      {
         pairs.push_back(std::pair<T, T>{ elements[i], elements[j] });
      }
   }
   return pairs;
}


/**
 * @brief check if the sum of any two numbers in the list equals the input value
 * @param value 
 * @param list 
 * @return 
*/
bool sum_contained_in( const int64_t value, const std::vector<int64_t>& list )
{
   auto pairs = get_pair_combinations( list );
   auto result = std::transform_reduce( pairs.cbegin(), pairs.cend(), 0L,
                                        std::plus<>(),
                                        [value](auto &p){ return ( p.first + p.second) == value; }
                                       );
   return static_cast<bool>(result);
}


/**
 * @brief process input data based on a fixed preamble size
 * @param values the values to process
 * @param preamble_size number of preamble bytes
 * @return valid entry for each element in the input vector
*/
std::vector<bool> process_from_preamble(std::vector<int64_t>& values, int64_t preamble_size)
{
   std::vector<bool> results( values.size() );
   std::fill(results.begin(), results.begin() + preamble_size, true );
   std::transform(values.cbegin() + preamble_size, values.cend(), results.begin() + preamble_size, 
      [preamble_size, values, count = 0]( const int &a) mutable
      {         
         auto sub_array = std::vector<int64_t>{values.begin() + count, values.begin() + preamble_size + count};
         count++;
         auto result = sum_contained_in(a, sub_array);
         return result;
      }
   );
   return results;
}

/****************************** Functions Definition ***********************************/
/**
 * @brief main application entry point
 * @param argc number of arguments
 * @param argv pointer to array of inputs
 * @return integer return code
*/
int main( int argc, char *argv[] )
{     
   std::ifstream stream{ argv[1] };
   std::string line;
   std::vector<int64_t> values;
   while (std::getline(stream, line))
   {
      values.push_back( std::stoll(line) );
   }

   /* part one solution */
   auto sum_in_previous = process_from_preamble( values, 25 );
   auto iterator = std::find( sum_in_previous.cbegin(), sum_in_previous.cend(), false );
   auto index = iterator - sum_in_previous.cbegin();
   int invalid_number = values[index];
   std::cout << "First value not contained in previous preamble " << invalid_number << std::endl;

   /* part two solution */
   std::vector<bool> contains_invalid_number;
   std::transform(values.cbegin(), values.cend(), std::back_inserter(contains_invalid_number),
      [invalid_number, values, count = 0](auto & val) mutable 
      {
         std::vector<int64_t> running_sum{ values.begin() + count, values.end() };
         std::partial_sum( running_sum.begin(), running_sum.end(), running_sum.begin());
         auto it = std::find(running_sum.cbegin(), running_sum.cend(), invalid_number );
         count++;
         return ( it != running_sum.end() );
      }
   );

   auto start_index = std::find(contains_invalid_number.cbegin(), contains_invalid_number.cend(), true ) - contains_invalid_number.cbegin();  

   /* gross while loop - could probably do with an STL algorithm? */
   int min_value = values[start_index];
   int max_value = values[start_index];
   int sum = 0;
   while (sum != invalid_number)
   {
      min_value = ( values[start_index] < min_value) ? values[start_index] : min_value;
      max_value = ( values[start_index] > max_value ) ? values[start_index] : max_value;
      sum += values[start_index++];
   }

   std::cout << "sum of largest and smallest values: " << min_value + max_value << "\n";
   
   return 0;
}
