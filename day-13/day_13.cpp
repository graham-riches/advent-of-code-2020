/*! \file day_13.cpp
*
*  \brief day-13 advent of code problem
* 
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include <algorithm>
#include <numeric>
#include <fstream>
#include <ostream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <cmath>
#include <cstdint>


/*********************************** Consts ********************************************/


/************************************ Types ********************************************/
struct Bus
{
   int64_t time_interval{0};
   int64_t offset{0};
};

struct BusScheduler
{
   int starting_timestamp{0};
   std::vector<Bus> busses;

   BusScheduler( int64_t start_time, std::vector<Bus> busses ) 
      : starting_timestamp(start_time), busses(busses)
   {};
};

/******************************** Local Variables **************************************/


/******************************** Classes and Structs **************************************/


/******************************** Function Declarations **************************************/
/**
    * @brief stript a substring out of the string
    * @input the input string
    * @param substring the substring to split
    * @return reference to self
   */
inline std::string& strip(std::string &input, const std::string& substring)
{
   size_t substring_pos = std::string::npos;
   while ((substring_pos = input.find(substring)) != std::string::npos)
   {
      input.erase(substring_pos, substring.length());
   }
   return input;
}

/**
 * @brief split a string object into sub strings based on a delimiter string
 * @param input the input string object
 * @param substring to split by
 * @return vector of strings
*/
inline std::vector<std::string> split(std::string &input, const std::string& substring)
{
   std::vector<std::string> list;
   size_t substring_pos = std::string::npos;
   while ((substring_pos = input.find(substring)) != std::string::npos)
   {
      list.push_back(input.substr(0, substring_pos));
      input.erase(0, substring_pos + substring.length());
   }
   list.push_back(input.substr(0, input.length()));
   return list;
}

/**
 * @brief function to load the data into a collection
 * @param filename filename to read input from
 * @return vector of operations + magnitude
*/
BusScheduler load_data(const std::string& filename)
{
   /* unique input data problem - only two lines of data */
   std::ifstream stream{ filename };
   std::string line;

   /* read the starting time stamp */
   std::getline(stream, line);
   int start_timestamp = std::stoll(line);

   /* read the available busses */
   std::getline(stream, line);
   std::vector<std::string> bus_strings = split(line, ",");

   std::vector<Bus> busses;   
   for (int i = 0; i < bus_strings.size(); i++)
   {
      if (bus_strings[i] != "x")
      {
         busses.push_back( Bus{ std::stoll(bus_strings[i]), i } );
      }
   }

   return BusScheduler{start_timestamp, busses};
}

/**
 * @brief get the bezout identity of a set of numbers
 * @param a first number
 * @param b second number
 * @return std::pair of (s, t) where -> s*a + t*v = gcd(a, b)
 * @note ignore the code gore ¯\_(ツ)_/¯ 
 *     for future reference: https://www.omnicalculator.com/math/chinese-remainder#modulo-operations-and-congruences                           
*/
std::pair<int64_t, int64_t> get_bezout_factors(const int64_t& a, const int64_t& b)
{
   /* define some constants to use as indices for at least a little clarity - kind of gross, but a little better than magic numbers */
   enum Indices
   {
      remainder = 0,
      quotient,
      bezout_s,
      bezout_t
   };

   /* always take x as the largest value */
   int64_t x = ( a > b ) ? a : b;
   int64_t y = ( a > b ) ? b : a;

   /* create a vector to store each iteration */
   std::vector<std::vector<int64_t>> iterations;

   /* order of vector is: remainder, quotient, bezout s, bezout t */
   iterations.push_back( std::vector<int64_t>{x, 0, 1, 0} );
   iterations.push_back( std::vector<int64_t>{y, x/y, 0, 1 } );
   
   /* start on the next iteration and run until the remainder is 0 */
   int64_t rem{1};
   int64_t it{2};
   while ( rem != 0 )
   {
      rem = iterations[it - 2][Indices::remainder] % iterations[it - 1][Indices::remainder];      
      int64_t s = iterations[it - 2][Indices::bezout_s] - (iterations[it - 1][Indices::quotient] * iterations[it - 1][Indices::bezout_s]);
      int64_t t = iterations[it - 2][Indices::bezout_t] - (iterations[it - 1][Indices::quotient] * iterations[it - 1][Indices::bezout_t]);
      int64_t quotient = ( rem != 0 ) ? iterations[it - 1][Indices::remainder] / rem : 0;
      iterations.push_back( std::vector<int64_t>{ rem, quotient, s, t } );
      it++;
   }

   /* values of interest are in the second last iteration (last one is junk data with remainder 0) */
   return std::pair<int64_t, int64_t>{iterations[iterations.size()-2][Indices::bezout_s], iterations[iterations.size() - 2][Indices::bezout_t]};
}

/**
 * @brief solve a remainder theorem problem
 * @param divisors system modulus coefficients
 * @param remainders remainder value for coefficient
 * @return first value that is the correct remainder multiple of all values
 * 
 * @note the total value is sum(ei*ai) where ei = bezout_s*Ni where Ni = (n1*n2*...)/ni
*/
int64_t solve_modulo_system(std::vector<int64_t>& divisors, std::vector<int64_t>& remainders)
{
   /* get the total modulo multiple: N*/
   auto N = std::accumulate( divisors.cbegin(), divisors.cend(), int64_t{1}, std::multiplies<int64_t>() );

   /* get the individual ni values */
   std::vector<int64_t> ni(divisors.size());
   std::transform( divisors.cbegin(), divisors.cend(), ni.begin(), [N](const int64_t &div){ return N/div;} );

   /* get the individual ei values */
   std::vector<int64_t> ei(divisors.size());
   auto solve_ei = [ni, count=0](const int64_t &divisor) mutable
   {
      auto bezout = get_bezout_factors( divisor, ni[count]);
      auto retval = bezout.first * ni[count];
      count++;
      return retval;
   };
   std::transform( divisors.cbegin(), divisors.cend(), ei.begin(), solve_ei );
   
   auto total_sum = std::inner_product( remainders.cbegin(), remainders.cend(), ei.begin(), int64_t{0});
   
   /* add N to the value until it is positive */
   while (total_sum < 0)
   {
      total_sum += N;
   }
   return total_sum % N;
}



/************************************ Classes ********************************************/


/****************************** Functions Definition ***********************************/
/**
 * @brief main application entry point
 * @param argc number of arguments
 * @param argv pointer to array of inputs
 * @return integer return code
*/
int main( int argc, char *argv[] )
{     
   BusScheduler scheduler = load_data(std::string{argv[1]});

   /*------------------------------ Part One Solution ------------------------------*/      
   /* Overview:
   *  - take a time slice [ start_time : 2 x (start_time + max(bus_id)) ], this ensures each bus will arrive at least once in the time window
   *  - the earliest time a bus arrives in the window is when the time_slot % bus_id == 0, so apply this in a lambda + reduction to get
   *    the minimum for each bus
   *  - take the minimum arrival time and subtract the start time : presto !
   */
      
   /* fill a uniform time slice [ start_time : 2 x (start_time + max(bus_id)) ] with monotonically increasing values */
   auto max_id = std::max_element(scheduler.busses.cbegin(), scheduler.busses.cend(), [](auto &a, auto &b){ return (a.time_interval < b.time_interval); } );
   std::vector<int> time_slots( 2L * (max_id->time_interval) );  
   std::iota(time_slots.begin(), time_slots.end(), scheduler.starting_timestamp);
   
   /* lambda to get the min of two elements */
   auto minimum = [](const int64_t &a, const int64_t &b ){ return std::min( a,b ); };

   /* lambda to get the first arrival time of a bus */
   auto get_earliest_arrival_time = [time_slots, minimum](const Bus& bus)
   {      
      return std::transform_reduce(time_slots.cbegin(), time_slots.cend(), time_slots.back(),
         minimum,
         [bus](const int64_t &timestamp)
         {
            return ( timestamp % bus.time_interval == 0 ) ? timestamp : std::numeric_limits<int>::max();
         }
         );
   };

   /* We also need the bus id so we have to use a transform first so that a local copy of all min arrivals times exists per bus.
      This prevents doing it all in one shot with transform_reduce :(
   */
   std::vector<int64_t> min_arrival_times;
   std::transform(scheduler.busses.cbegin(), scheduler.busses.cend(), std::back_inserter(min_arrival_times), get_earliest_arrival_time );

   auto min_bus_time = std::min_element(min_arrival_times.cbegin(), min_arrival_times.cend());   
   auto bus_id = scheduler.busses[min_bus_time - min_arrival_times.begin()].time_interval;
   auto wait_time = *min_bus_time - scheduler.starting_timestamp;

   std::cout << "Minimum wait time: " << wait_time * bus_id << "\n\n";



   /*------------------------------ Part Two Solution ------------------------------*/
   std::vector<int64_t> divisors(scheduler.busses.size());
   std::vector<int64_t> remainders(scheduler.busses.size());
   std::transform(scheduler.busses.cbegin(), scheduler.busses.cend(), divisors.begin(), [](const Bus &bus){ return bus.time_interval; }); 
   std::transform(scheduler.busses.cbegin(), scheduler.busses.cend(), remainders.begin(), [](const Bus& bus) { return -bus.offset; });

   auto magic_time = solve_modulo_system( divisors, remainders );

   std::cout << "time slot is: " << magic_time << "\n";
   return 0;
}
