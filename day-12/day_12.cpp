/*! \file day_12.cpp
*
*  \brief day-12 advent of code problem
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


/*********************************** Consts ********************************************/
constexpr double pi = 3.14159265358979323846;

/************************************ Types ********************************************/
/**
 * @brief enumeration of all possible directions
*/
enum class Operations : unsigned
{
   move_north,
   move_south,
   move_east,
   move_west,
   rotate_left,
   rotate_right,
   move_forward,
};

/**
 * @brief enumeration of possible ship orientations
*/
enum class Bearing : unsigned
{
   north,
   east,
   south,
   west
};


/******************************** Local Variables **************************************/
static const std::map<std::string, Operations> operation_map{{"N", Operations::move_north}, {"S", Operations::move_south},  {"E", Operations::move_east},
                                                             {"W", Operations::move_west},  {"L", Operations::rotate_left}, {"R", Operations::rotate_right},
                                                             {"F", Operations::move_forward} };                                                      

static const std::map<Operations, Bearing> bearing_map{{Operations::move_north, Bearing::north}, {Operations::move_south, Bearing::south},
                                                       {Operations::move_east, Bearing::east}, {Operations::move_west, Bearing::west}};

/* two maps of orientation to bearing, and vice versa just for convenience */
static const std::map<int, Bearing> angle_to_bearing{{0, Bearing::east}, {90, Bearing::north}, {180, Bearing::west}, {270, Bearing::south} };
static const std::map<Bearing, int> bearing_to_angle{ {Bearing::east, 0}, {Bearing::north, 90}, {Bearing::west, 180}, {Bearing::south, 270} };


/******************************** Classes and Structs **************************************/
/**
 * @brief struct to handle the current location and heading of the ship
*/
struct Location
{
   int bearing_degrees;
   int x_position;
   int y_position;

   Location(Bearing bearing, int x, int y) :x_position(x), y_position(y)
   {
      this->bearing_degrees = bearing_to_angle.at(bearing);
   };
};

/**
 * @brief struct to handle the waypoint
*/
struct Waypoint
{
   int x_position;
   int y_position;
   Waypoint( int x, int y ) : x_position(x), y_position(y) {};
};

/******************************** Function Declarations **************************************/
/**
 * @brief function to load the data into a collection
 * @param filename filename to read input from
 * @return vector of operations + magnitude
*/
std::vector<std::pair<Operations, int>> load_data(const std::string& filename)
{
   std::ifstream stream{ filename };
   std::string line;
   std::vector<std::pair<Operations,int>> instructions;
   while (std::getline(stream, line))
   {
      std::smatch matches;
      std::regex_search(line, matches, std::basic_regex{"(\\w)(\\d+)", std::regex::icase});
      instructions.push_back( std::pair{operation_map.at(std::string{matches[1]}), std::stoi(matches[2])} );
   }
   return instructions;
}

/**
 * @brief move the ship a certain direction by a set distance 
 * @param location the current location of the ship
 * @param bearing, the direction to move
 * @param magnitude 
*/
void move(Location& location, Bearing bearing, int magnitude)
{
   switch (bearing)
   {
      case Bearing::east:
         location.x_position += magnitude;
         break;

      case Bearing::west:
         location.x_position -= magnitude;
         break;

      case Bearing::north:
         location.y_position += magnitude;
         break;

      case Bearing::south:
         location.y_position -= magnitude;
         break;

      default:
         break;
   }
}

/**
 * @brief move the waypoint relative to the ship
 * @param waypoint the current waypoint location
 * @param bearing, the direction to move
 * @param magnitude
*/
void move_waypoint(Waypoint& waypoint, Bearing bearing, int magnitude)
{
   switch (bearing)
   {
   case Bearing::east:
      waypoint.x_position += magnitude;
      break;

   case Bearing::west:
      waypoint.x_position -= magnitude;
      break;

   case Bearing::north:
      waypoint.y_position += magnitude;
      break;

   case Bearing::south:
      waypoint.y_position -= magnitude;
      break;

   default:
      break;
   }
}

/**
 * @brief move the ship towards the waypoint
 * @param location current location of the ship
 * @param waypoint waypoint location
 * @param magnitude number of times to move towards the waypoing
*/
void move_to_waypoint(Location& location, Waypoint waypoint, int magnitude)
{
   location.x_position += waypoint.x_position * magnitude;
   location.y_position += waypoint.y_position * magnitude;
}

/**
 * @brief apply a rotation to the ship
 * @param location the current location
 * @param angle amount to rotate by
*/
void rotate(Location& location, int angle)
{
   location.bearing_degrees = (location.bearing_degrees + angle);
   while (location.bearing_degrees < 0)
   {
      location.bearing_degrees += 360;
   }
   location.bearing_degrees = location.bearing_degrees % 360;
}

/**
 * @brief rotate the waypoint about the ship
 * @param waypoint the waypoint
 * @param angle amount to rotate by
 * @note: this does some dubious floating point math :D
*/
void rotate_waypoint(Waypoint& waypoint, int rotation_angle) 
{   
   double radius = std::sqrt(std::pow(waypoint.x_position, 2) + std::pow(waypoint.y_position, 2));
   double angle = std::atan2( waypoint.y_position, waypoint.x_position );
   angle += static_cast<double>( rotation_angle * pi / 180.00);

   waypoint.x_position = static_cast<int>( std::round( std::cos(angle) * radius ));
   waypoint.y_position = static_cast<int>( std::round( std::sin(angle) * radius ));   
}

/**
 * @brief handle a current instruction
 * @param location the current location of the ship
 * @param operation what operation to apply
 * @param magnitude magnitude of the operation
*/
void apply_part_one_operations(Location &location, Operations operation, int magnitude)
{
   switch (operation)
   {
      /* intentional fallthrough */
      case Operations::move_east:
      case Operations::move_west:
      case Operations::move_north:
      case Operations::move_south:
         move(location, bearing_map.at(operation), magnitude);
         break;

      case Operations::move_forward:
         move(location, angle_to_bearing.at(location.bearing_degrees), magnitude);
         break;

      case Operations::rotate_left:
         rotate(location, magnitude);
         break;

      case Operations::rotate_right:
         rotate(location, -magnitude);
         break;

      default:
         break;
   }
}


/**
 * @brief apply the rules for part two
 * @param location current ship location
 * @param waypoint current waypoint location relative to the ship
 * @param operation the operation to perform
 * @param magnitude size of the operation
*/
void apply_part_two_operations(Location& location, Waypoint &waypoint, Operations operation, int magnitude)
{
   switch (operation)
   {
      /* intentional fallthrough */
      case Operations::move_east:
      case Operations::move_west:
      case Operations::move_north:
      case Operations::move_south:
         move_waypoint(waypoint, bearing_map.at(operation), magnitude);
         break;

      case Operations::move_forward:
         move_to_waypoint(location, waypoint, magnitude);
         break;

      case Operations::rotate_left:
         rotate_waypoint(waypoint, magnitude);
         break;

      case Operations::rotate_right:
         rotate_waypoint(waypoint, -magnitude);
         break;

      default:
         break;
   }
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
   auto instructions = load_data(std::string{argv[1]});
   Location location{Bearing::east, 0, 0};
   
   /*------------------------------ Part One Solution ------------------------------ */
   for (auto& instruction : instructions)
   {
      apply_part_one_operations( location, instruction.first, instruction.second );
   }
   std::cout << "Manhattan Distance: " << std::abs(location.x_position) + std::abs(location.y_position) << "\n";



   /*------------------------------ Part Two Solution ------------------------------ */
   location = Location{Bearing::east, 0, 0};
   Waypoint waypoint{10, 1};
   for (auto& instruction : instructions)
   {
      apply_part_two_operations(location, waypoint, instruction.first, instruction.second);
   }

   std::cout << "Manhattan Distance: " << std::abs(location.x_position) + std::abs(location.y_position) << "\n";
   return 0;
}



