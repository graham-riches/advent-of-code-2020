/*! \file day_11.cpp
*
*  \brief day-11 advent of code problem
* 
*  \NOTE: this solution is kind of gross. Especially part two!
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
#include <functional>


/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * @brief possible values for a seat location
*/
enum class LocationStatus : unsigned
{
   empty = 0,
   occupied,
   floor,
};


/******************************** Local Variables **************************************/
/* store two maps (for convenience): one from char to seat location, and the other for seat location to char (debug print)*/
static const std::map<char, LocationStatus> char_to_seat{ {'L', LocationStatus::empty}, {'#', LocationStatus::occupied}, {'.', LocationStatus::floor} };
static const std::map<LocationStatus, char> seat_to_char{ {LocationStatus::empty, 'L'}, {LocationStatus::occupied, '#'}, {LocationStatus::floor, '.'} };
static const std::vector<std::pair<int, int>> view_directions{ {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};


/******************************** Function Declarations **************************************/



/************************************ Classes ********************************************/
struct Seats
{
   std::vector<std::vector<LocationStatus>> locations;
   int rows{0};
   int columns{0};

   /**
    * @brief construct a seats struct from a file input
    * @param filename the file input
   */
   Seats(const std::string& filename)
   {      
      std::ifstream stream{ filename };
      std::string line;      
      while (std::getline(stream, line))
      {
         std::vector<LocationStatus> row_values;
         std::transform(line.cbegin(), line.cend(), std::back_inserter(row_values), [](const char &c){ return char_to_seat.at(c); } );         
         this->locations.push_back( row_values );
         this->rows++;
      }
      this->columns = line.length();
   }

   /**
    * @brief tally all seats matching the selection criteria
    * @param status the status selection
    * @return count of seats matching the selection
   */
   int get_seats_by_status(LocationStatus status)
   {
      auto count = std::transform_reduce(this->locations.cbegin(), this->locations.cend(), 0L, std::plus<>(),
         [status](const std::vector<LocationStatus>& row)
         {
            return std::count_if( row.cbegin(), row.cend(), 
               [status]( const LocationStatus &location ){ return ( location == status);}
            );
         }
      );
      return count;
   }

   /**
    * @brief check if a location is in bounds
    * @param row 
    * @param column 
    * @return true if it's an edge
   */
   inline bool is_in_bounds(int row, int column)
   {
      bool in_bounds{true};
      if ((row < 0) || (row >= this->rows) || (column < 0) || (column >= this->columns))
      {
         in_bounds = false;
      }
      return in_bounds;
   }

   /**
    * @brief get a vector of all neighbours to a seat
    * @param row the row index
    * @param column the column index
    * @return vector of neighbours
   */
   std::vector<LocationStatus> get_neighbours(int row, int column)
   {
      std::vector<LocationStatus> neighbours;

      /* ewwwww I should probably find a cleaner way to do this */
      int row_start = ( row == 0 ) ? 0 : row - 1;
      int col_start = ( column == 0 )? 0 : column - 1;
      int row_end = ( row == this->rows - 1 ) ? this->rows - 1 : row + 1;
      int col_end = ( column == this->columns - 1) ? this->columns - 1 : column + 1;

      for (int i = row_start; i <= row_end; i++)
      {
         for (int j = col_start; j <= col_end; j++)
         {
            if ((i != row) || (j != column))
            {
               neighbours.push_back(this->locations[i][j]);
            }
            
         }
      }
      return neighbours;
   }

   /**
    * @brief get the seat status in each direction fanning out from the current location
    * @param row_start current row
    * @param column_start current column
    * @return vector of what is seen in each direction
   */
   std::vector<LocationStatus> count_in_all_directions(int row_start, int column_start)
   {
      std::vector<LocationStatus> directions;
      for (auto& direction : view_directions)
      {
         int row{row_start};
         int column{column_start};

         /* loop over the view direction until either an edge is encountered, or a seat view is found */
         while (true)
         {
            row += direction.first;
            column += direction.second;
            if (this->is_in_bounds(row, column))
            {
               auto status = this->locations[row][column];
               if (status != LocationStatus::floor)
               {
                  directions.push_back(status);
                  break;
               }
            }
            else
            {
               directions.push_back(LocationStatus::floor);
               break;
            }
         }
      }
      return directions;
   }

   /**
    * @brief apply the rules to a vector of visible seats at a location
    * @param starting_status the initial value for a location
    * @param seats the vector of seats
    * @param empty_threshold how many occupied seats nearby to trigger an emptying action
    * @return 
   */
   LocationStatus apply_rules(LocationStatus starting_status, std::vector<LocationStatus>& seats, int empty_threshold)
   {
      auto count = std::count_if(seats.cbegin(), seats.cend(), [](auto& n) { return n == LocationStatus::occupied; });

      LocationStatus new_status{ starting_status };
      if ((count >= empty_threshold) && (starting_status == LocationStatus::occupied))
      {
         new_status = LocationStatus::empty;
      }
      else if ((count == 0) && (starting_status == LocationStatus::empty))
      {
         new_status = LocationStatus::occupied;
      }
      return new_status;
   }

   /**
    * @brief apply the simulation rules at a location and return the new status
    * @param row the current row
    * @param column the current column
    * @return new simulation status
   */
   LocationStatus rules_part_one(int row, int column)
   {
      auto current_status = this->locations[row][column];
      auto neighbours = this->get_neighbours(row, column);
      
      return this->apply_rules(current_status, neighbours, 4);
   }

   /**
    * @brief apply the second batch of simulation rules and return the new status
    * @param row current row
    * @param column current column
    * @return new status
   */
   LocationStatus rules_part_two(int row, int column)
   {
      auto current_status = this->locations[row][column];
      auto visible_seats = this->count_in_all_directions(row, column);
      return this->apply_rules(current_status, visible_seats, 5);
   }

   /**
    * @brief run a complete iteration
    * @note: this should really use a function pointer or something to select which rule set to call
   */
   void run_iteration(void)
   {
      /* store a copy of the data and store updated locations there */
      std::vector<std::vector<LocationStatus>> new_locations;

      for (int row = 0; row < this->rows; row++)
      {
         std::vector<LocationStatus> new_row;
         for (int column = 0; column < this->columns; column++)
         {
            new_row.push_back(this->rules_part_two(row, column));
         }
         new_locations.push_back(new_row);
      }
      this->locations = new_locations;
   }
   
   /**
    * @brief output the seat locations
    * @param os an output stream to put the seat data into
    * @param seats reference to the seats object
    * @return the output steam with data added
   */
   friend std::ostream& operator << (std::ostream& os, const Seats& seats)
   {
      for (auto& row : seats.locations)
      {
         for (auto& col : row)
         {
            os << seat_to_char.at(col);
         }
         os << "\n";
      }
      return os;
   }
};





/****************************** Functions Definition ***********************************/
/**
 * @brief main application entry point
 * @param argc number of arguments
 * @param argv pointer to array of inputs
 * @return integer return code
*/
int main( int argc, char *argv[] )
{     
   Seats seats{std::string{argv[1]}}; 
   
   int seats_available = seats.get_seats_by_status(LocationStatus::empty);
   int new_seats_available{0};

   /* run the simulation until it stabilizes */
   while ( new_seats_available != seats_available )
   {
      seats_available = seats.get_seats_by_status(LocationStatus::empty);
      seats.run_iteration();
      new_seats_available = seats.get_seats_by_status(LocationStatus::empty);      
   }

   std::cout << "Empty seat counts: " << seats.get_seats_by_status(LocationStatus::empty) << "\n"
             << "Occupied seat count: " << seats.get_seats_by_status(LocationStatus::occupied) << "\n"
             << "Floor spaces: " << seats.get_seats_by_status(LocationStatus::floor) << "\n";

   return 0;
}
