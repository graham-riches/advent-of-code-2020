/*! \file day_3.cpp
*
*  \brief day-3 advent of code problem
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

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/


/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype **************************************/
int binary_search( std::vector<int> &input );

/****************************** Functions Definition ***********************************/
/**
 * @brief main application entry point
 * @param argc number of arguments
 * @param argv pointer to array of inputs
 * @return integer return code
*/
int main( int argc, char *argv[] )
{
   std::ifstream seat_data{ std::string{ argv[1] } };
   std::string line;
   std::vector<int> seats;
   while ( std::getline( seat_data, line ) )
   {
      std::vector<int> binary_data(line.length());
      std::transform( cbegin(line), cend(line), binary_data.begin(), [](const char c){ return ( (c == 'F') || (c == 'L')); } );

      std::vector<int> row_data{ binary_data.begin(), binary_data.begin() + 7 };
      std::vector<int> col_data{ binary_data.begin() + 7, binary_data.end() };
      auto row = binary_search(row_data);
      auto col = binary_search(col_data);
      seats.push_back( row*8 + col );
   }

   /* calculate the seat id */
   auto max_id = std::max_element( cbegin(seats), cend(seats) );
   std::cout << "max seat id " << *max_id << std::endl;

   /* sort, then use adjacent difference */
   std::sort(seats.begin(), seats.end());
   std::vector<int> diffs;
   auto seat_bounds = std::adjacent_difference(cbegin(seats), cend(seats), std::back_inserter(diffs) );
   auto index = std::find(begin(diffs), end(diffs), 2) - diffs.begin();
   std::cout << "seat ID is " << seats[index] - 1;

   return 0;
}


/**
 * @brief performs a binary lower bound search on a data collection and outputs the final value
 * @param input 
 * @return 
*/
int binary_search(std::vector<int>& input)
{
   int range = 0x01 << input.size();
   std::transform(begin(input), end(input), begin(input),
      [range, iter = 1, accumulate = 0](bool lower_bound) mutable
      {
         int value = ( lower_bound ) ? accumulate : (range / (0x01 << iter)) + accumulate;
         accumulate = value;
         iter++;
         return value;
      });
   return input.back();
}