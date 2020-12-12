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
#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include <cmath>


/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype **************************************/
/**
 * @brief function to load the data into a collection
 * @param filename filename to read input from
 * @return vector of ints
*/
std::vector<int> load_data(const std::string& filename)
{   
   std::ifstream stream{ filename };
   std::string line;
   std::vector<int> values;
   while (std::getline(stream, line))
   {
      values.push_back(std::stoi(line));
   }
   return values;
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
   /*------------------------------ Load data, sort, etc. ------------------------------*/   
   std::vector<int> values = load_data(std::string{argv[1]});

   /* append the start and end values: 0 jolt to start, and max + 3 jolt to end */
   values.push_back( 0 );
   values.push_back( *std::max_element(values.begin(), values.end()) + 3 );

   /* sort the values */
   std::sort(values.begin(), values.end());



   /*------------------------------ Part One Solution ------------------------------*/
   /* Overview:
   *  - take the adjacent difference of each element
   *  - create a lambda that transforms each element in the collection to 1 (value contained) or 0 and
   *    reduces the resulting collection to a single value (transform_reduce)         
   */
   std::vector<int> difference(values.size());
   std::adjacent_difference( values.cbegin(), values.cend(), difference.begin());

   auto tally_deltas_at = [difference](int delta)
   {
      return std::transform_reduce(difference.cbegin(), difference.cend(), 0L, std::plus<>(),
         [delta](auto &difference){ return ( delta == difference ); }
      );
   };
   
   std::cout << "Multiple of 1-jolt and 3-jolt differences: " << tally_deltas_at(1) * tally_deltas_at(3) << "\n";



   /*------------------------------ Part Two Solution ------------------------------*/
   /* Overview: (this one is a bit spicy)
   *  - permutations on the main set are based on a set of filters that generate a specific number of new permutations
   *  - store the total count of how many times each filter is detected as a subset of the collection
   *  - the total number of permutations is total = reduce_multiply[ (filter possible combinations)^(filter instances) | filter in filters ]
   */         
   /* lambda to check if a collection begins with a another collection*/
   auto starts_with_filter = [](const std::vector<int>& set, const std::vector<int>& filter)
   {      
      return ( set.size() >= filter.size() ) ? std::inner_product(filter.cbegin(), filter.cend(), set.cbegin(), 1L, std::multiplies<>(), std::equal_to<>()) : 0;      
   };

   /* lambda to check each a set of filters against a collection - returns a vector of count of each filter */
   auto count_filters = [starts_with_filter](std::vector<int>& collection, const std::vector<std::vector<int>> &filters) 
      {
         std::vector<int> count(filters.size());
         std::transform( filters.cbegin(), filters.cend(), count.begin(), [collection, starts_with_filter](auto& filter)
            {
               return starts_with_filter(collection, filter);
            }
         );
         return count;
      };
     
   /* filters for each combination */
   const std::vector<std::vector<int>> filters{ {std::vector<int>{3, 1, 1, 3    }},
                                                {std::vector<int>{3, 1, 1, 1, 3 }},
                                                {std::vector<int>{1, 1, 1, 1    }} };
   
   std::vector<int> filter_count(filters.size());  //!< total number of each filter instance
   std::vector<int> combination_values{2, 4, 7};   //!< combination multiple of each filter
   
   /* scan through the list and count up the filter instances */
   for ( auto it = difference.begin(); it != difference.end(); it++ )
   {
      std::vector<int> collection = std::vector<int>{it, difference.end()};
      auto count = count_filters(collection, filters);      
      std::transform(count.begin(), count.end(), filter_count.begin(), filter_count.begin(), std::plus<>());
   }
      
   /* reduce the list by multiplying every combination_value^filter_instances together : beware of int overflows :D */
   auto combinations = std::transform_reduce(filter_count.begin(), filter_count.end(), int64_t{1}, std::multiplies<int64_t>(), 
      [combination_values, it=0](auto &power) mutable
      { 
         auto multiple = std::powl(combination_values[it], power);
         it++;
         return static_cast<int64_t>(multiple);
      }
   );

   std::cout << "total permutations: " << combinations << "\n";

   return 0;
}
