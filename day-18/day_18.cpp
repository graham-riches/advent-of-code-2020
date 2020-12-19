/*! \file day_18.cpp
*
*  \brief day-18 advent of code problem
* 
*
*  \author Graham Riches
*  \details basic additional and multiplication with some modified rules for handling
*           order of operations
*           - scan for open parenthesis -> get the substring inside of this and evaluate it
*           - evaluate left to right
*           - use a lambda to change the callable at every next iteration
*/

/********************************** Includes *******************************************/
#include <algorithm>
#include <memory>
#include <numeric>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <set>
#include <cstdint>
#include <chrono>
#include "string_utilities.h"


/************************************ Types ********************************************/
class Operation
{
public:
   Operation(const char type) : type(type) {};

   Operation( const Operation& operation )
   {
      this->type = operation.type;
   }

   /**
    * @brief overload the callable operator to handle multiply or sum
    * @param a arg one
    * @param b arg two
    * @return output result
   */
   int64_t operator() ( int64_t a, int64_t b ) const
   {
      return (this->type == '*') ? a * b : a + b;
   }

   /**
    * @brief get the operation type
    * @param  
    * @return 
   */
   char get_type( void ) const
   {
      return this->type;
   }

private:
   char type;
};


struct MathSequence
{
   std::vector<int64_t> numbers;
   std::vector<Operation> operations;
   MathSequence( std::vector<int64_t> &numbers, std::vector<Operation> &operations ) 
   : numbers(std::move(numbers)), operations(std::move(operations))
   { }
};


/****************************** Functions Definition ***********************************/
/**
 * @brief parse a line recursively with a templated function object
 * @tparam callable template function object that can process a math sequence
 * @param line string input line
 * @return line value
*/
template <class F>
int64_t parse_math_line( std::string& line, F callable )
{
   line = strip( line, " " );

   std::vector<int64_t> numbers;
   std::vector<Operation> operations;

   std::string::size_type iterator{ 0 };
   while (iterator < line.size())
   {
      if (line[iterator] == '(')
      {
         std::string::size_type end_paren{ iterator };
         std::string::size_type open_parens{ 0 };
         std::string::size_type close_parens{ 0 };
         while (end_paren != line.size())
         {
            if (line[end_paren] == '(')
            {
               open_parens++;
            }
            if (line[end_paren] == ')')
            {
               close_parens++;
            }
            if (open_parens == close_parens)
            {
               break;
            }
            end_paren++;
         }

         /* recursively call this function to parse substrings inside parenthesis */
         std::string new_line = line.substr( iterator + 1, end_paren - iterator - 1 );
         numbers.push_back( parse_math_line( new_line, callable ) );
         iterator = end_paren;
      }
      else
      {
         switch (line[iterator])
         {
         case '+':
         case '*':
            operations.push_back( line[iterator] );
            break;

         default:
            numbers.push_back( std::stoi( std::string{ line[iterator] } ) );
            break;
         }
      }
      iterator++;
   }

   MathSequence sequence{ numbers, operations };   
   return callable(sequence);
}


/**
 * @brief function to convert a math sequence to an integer following the rules in part one
 * @param sequence the sequence to convert
 * @return the output value
*/
int64_t accumulate_sequence_part_one( MathSequence& sequence )
{
   int64_t output = std::accumulate( sequence.operations.begin(), sequence.operations.end(), sequence.numbers[0],
      [sequence, count = 1]( auto accumulator, auto operation ) mutable
      {
         return operation( accumulator, sequence.numbers[count++] );
      }
   );
   return output;
}

/**
 * @brief convert a math sequence according to the swapped order of operations in part two
 * @param sequence the sequence
 * @return 
 * @note this function is heinous and probably could use some work
*/
int64_t accumulate_sequence_part_two( MathSequence& sequence )
{   
   int64_t i{ 0 };
   while ( true )
   {
      if ( sequence.operations[i].get_type() == '+' )
      {
         sequence.numbers[i] = sequence.numbers[i] + sequence.numbers[i + 1LL];
         sequence.numbers.erase( sequence.numbers.begin() + i + 1 );
         sequence.operations.erase( sequence.operations.begin() + i );
         i--;
      }
      i++;
      if (i == sequence.operations.size())
      {
         break;
      }
   }

   int64_t output = std::accumulate( sequence.numbers.begin(), sequence.numbers.end(), int64_t{ 1 }, std::multiplies<int64_t>() );
   return output;
}



/**
 * @brief main application entry point
 * @param argc number of arguments
 * @param argv pointer to array of inputs
 * @return integer return codeout
 * 
 * @note command line argument is the path to the directory containing the data
*/
int main( int64_t argc, char *argv[] )
{      
   auto start = std::chrono::steady_clock::now();

   std::vector<std::string> lines = read_file(open_file(argv[1]));

   /* part one solution */
   auto fold_equation_part_one = [](std::string &line)
   {
      return parse_math_line(line, accumulate_sequence_part_one);
   };

   auto result_part_one = std::transform_reduce( lines.begin(), lines.end(), int64_t{0}, std::plus<int64_t>(), fold_equation_part_one );


   /* part two solution */
   auto fold_equation_part_two = []( std::string& line )
   {
      return parse_math_line( line, accumulate_sequence_part_two );
   };

   auto result_part_two = std::transform_reduce( lines.begin(), lines.end(), int64_t{ 0 }, std::plus<int64_t>(), fold_equation_part_two );


   /* Display Results */
   auto end = std::chrono::steady_clock::now();

   std::cout << "Part One Sum: " << result_part_one  << "\n"
             << "Part Two Sum: " << result_part_two << "\n"
             << "Elapsed Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds \n";

   return 0;
}
