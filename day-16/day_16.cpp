/*! \file day_16.cpp
*
*  \brief day-16 advent of code problem
* 
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <ranges>
#include "string_utilities.h"


/************************************ Types ********************************************/
/**
 * @brief field object to represent the ticket fields
*/
class Field
{
private:
   std::pair<int, int> lower_range{0, 0};
   std::pair<int, int> upper_range{0, 0};
   std::string name;                

public:
   /**
      * @brief create a new Field object
      * @param name the name of the field
      * @param lower the lower bound pair
      * @param upper the upper bound pair
   */
   Field(std::string name, std::pair<int, int> lower, std::pair<int, int> upper)
      : name(name), lower_range(lower), upper_range(upper)
   {}

   /**
      * @brief default Field object constructor
   */
   Field() {};
          
   /**
      * @brief factory method to create a new Field object from an input string
      * @param input the input string
      * @return new field object
   */
   static Field from_string(std::string& input)
   {
      std::smatch matches;        
      std::regex_search(input, matches, std::regex{ "([a-zA-Z\\s]+): (\\d+)-(\\d+) or (\\d+)-(\\d+)", std::regex::icase });
      if (matches.size() == 6)
      {
         std::vector<std::string> string_matches(matches.size());
         std::transform( matches.begin(), matches.end(), string_matches.begin(), [](auto a){return std::string{a};} );
         std::string field_name{string_matches[1]};
         std::pair<int, int> lower_bound{ std::stoi(string_matches[2]), std::stoi(string_matches[3]) };
         std::pair<int, int> upper_bound{ std::stoi(string_matches[4]), std::stoi(string_matches[5]) };
         Field new_field(field_name, lower_bound, upper_bound);
         return new_field;
      }
   }

   /**
      * @brief check if an item is contained within the field bounds     
      * @param item 
      * @return 
   */
   bool item_in_bounds(int item) const
   {
      bool item_valid = (item >= lower_range.first && item <= lower_range.second) || (item >= upper_range.first && item <= upper_range.second);
      return item_valid;
   }

   /**
      * @brief check if two field objects are equal
      * @param field 
      * @return 
   */
   bool is_equal(const Field& field) const
   {
      return this->name == field.name;
   }
};


/****************************** Functions Definition ***********************************/
/**
 * @brief check if an value is valid for any field in a collection of fields
 * @param entry the entry to validate
 * @param fields collection of field objects
 * @return true if the entry is valid for any field
*/
bool is_entry_valid_for_any_field(int entry, const std::vector<Field> fields)
{
   std::vector<bool> fields_valid(fields.size());
   std::transform(fields.cbegin(), fields.cend(), fields_valid.begin(), [entry](const Field field) { return field.item_in_bounds(entry); });
   int count = std::count(fields_valid.begin(), fields_valid.end(), false);
   return (count == fields.size()) ? false : true;
}


/**
 * @brief check a ticket against a vector of fields
 * @param ticket the ticket
 * @param fields to check
 * @return true if ticket is valid
 * @note this checks every entry on the ticket against all fields. If an entry does not fit in any field,
 * the ticket is invalid
*/
bool is_ticket_valid(std::vector<int> ticket, const std::vector<Field> fields)
{
   std::vector<bool> ticket_entry_valid(ticket.size());
   auto check_entry = [fields]( int entry )
   {
      return is_entry_valid_for_any_field(entry, fields);
   };

   std::transform(ticket.begin(), ticket.end(), ticket_entry_valid.begin(), check_entry);
   auto count = std::count(ticket_entry_valid.begin(), ticket_entry_valid.end(), false );
   return ( count > 0 ) ? false : true;
}


/**
 * @brief get field information from a vector of strings
 * @param field_data field information encoded as strings
 * @return vector of field objects
*/
std::vector<Field> get_fields(std::vector<std::string> field_data)
{
   std::vector<Field> fields;
   std::transform(field_data.begin(), field_data.end(), std::back_inserter(fields), [](auto input) { return Field::from_string(input); });
   return fields;
}

/**
 * @brief convert a string encoding of a ticket into a vector of ints
 * @param line containing the string encoding of the ticket
 * @return vector of ints
*/
std::vector<int> string_to_ticket(std::string line)
{
   std::vector<std::string> string_data = split(line, ",");
   std::vector<int> ticket(string_data.size());
   std::transform(string_data.begin(), string_data.end(), ticket.begin(), [](std::string& val) { return std::stoi(val); });
   return ticket;
}


/**
 * @brief main application entry point
 * @param argc number of arguments
 * @param argv pointer to array of inputs
 * @return integer return code
 * @note command line argument is the path to the directory containing the data
*/
int main( int argc, char *argv[] )
{
   /* open all files and read all data: */
   const std::string filepath = std::string{argv[1]};   
   std::vector<std::string> files{"rules.txt", "ticket.txt", "nearby.txt"};
   auto prepend_filepath = [filepath]( std::string file){ return file.insert(0, filepath);};   
   auto inputs = files | std::views::transform(prepend_filepath) | std::views::transform(open_file) | std::views::transform(read_file);

   /* get the field information */
   std::vector<Field> fields = get_fields(inputs[0]);   

   /* get your ticket information */
   std::vector<int> ticket = string_to_ticket(inputs[1][0]);
         
   /* get all nearby tickets */
   std::vector<std::string> nearby_file_data = inputs[2];
   std::vector<std::vector<int>> nearby_tickets;
   std::transform(nearby_file_data.begin(), nearby_file_data.end(), std::back_inserter(nearby_tickets), string_to_ticket);
   



   /*------------------------------ part one - count invalid tickets ------------------------------*/
   /* get all invalid tickets */
   std::vector<std::vector<int>> invalid_tickets;
   std::copy_if(nearby_tickets.cbegin(), nearby_tickets.cend(), std::back_inserter(invalid_tickets),
      [fields](const std::vector<int> ticket){return !is_ticket_valid(ticket, fields);} );

   /* sum the invalid entries */
   auto total_of_invalid_entries = std::transform_reduce( invalid_tickets.cbegin(), invalid_tickets.cend(), 0L, std::plus<>(), 
      [fields](const std::vector<int> ticket)
      {
         auto invalid_entry = std::find_if( ticket.cbegin(), ticket.cend(), 
            [fields](const int entry){ return !is_entry_valid_for_any_field(entry, fields); } );
         return ( invalid_entry != ticket.cend() ) ? *invalid_entry : 0;
      } 
   );
   std::cout << "Sum of invalid ticket entries: " << total_of_invalid_entries << "\n";




   /*------------------------------ part two - count invalid tickets ------------------------------*/      
   /* remove all invalid entries including zeros */
   nearby_tickets.erase( std::remove_if( nearby_tickets.begin(), nearby_tickets.end(), 
      [fields](std::vector<int> ticket){ return !is_ticket_valid(ticket, fields);} ), nearby_tickets.end() );


   /* create a vector of valid fields for each column - initially all fields are valid for all columns */
   std::vector<std::vector<Field>> initial_fields_per_column(nearby_tickets[0].size());
   std::transform(initial_fields_per_column.begin(), initial_fields_per_column.end(), initial_fields_per_column.begin(),
      [fields](auto fields_per_entry )
      {
         std::vector<Field> all_fields = fields;
         return all_fields;
      });


   /* use a lambda and fold the validity vectors over each row entry dropping any entries that are invalid. 
      The final resulting vector should contain the fields for index.
   */
   auto drop_invalid_fields = [](std::vector<std::vector<Field>> fields, std::vector<int> ticket)
   {
      std::vector<std::vector<Field>> new_fields = fields;

      auto drop_fields_not_matching_ticket = [count = 0, ticket]( auto field_vector ) mutable
      {
         std::vector<Field> valid_fields = field_vector;

         valid_fields.erase( std::remove_if( valid_fields.begin(), valid_fields.end(), [ticket, count]( Field f ){ return !f.item_in_bounds(ticket[count]);} ),
                             valid_fields.end());
         count++;
         return valid_fields;
      };

      std::transform(new_fields.begin(), new_fields.end(), new_fields.begin(), drop_fields_not_matching_ticket);
      return new_fields;
   };

   /* using the above lambda and a fold, keep reducing the set of fields for each column until the end of the tickets collection is reached */
   auto valid_fields_per_column = std::accumulate(nearby_tickets.cbegin(), nearby_tickets.cend(), initial_fields_per_column, drop_invalid_fields );

   /* create a vector of column indicices to zip with the field entries */
   std::vector<int> columns(valid_fields_per_column.size());
   std::iota(columns.begin(), columns.end(), 0);
   
   using column_pair = std::pair<std::vector<Field>, int>;
   std::vector<column_pair> columns_with_data(columns.size());
   std::transform(columns.cbegin(), columns.cend(), columns_with_data.begin(), 
      [valid_fields_per_column](const int col){ return std::pair{valid_fields_per_column[col], col}; } );

   /* sort the set */
   std::sort( columns_with_data.begin(), columns_with_data.end(), [](column_pair a, column_pair b){ return (a.first.size() < b.first.size()); } );

   /* get the sorted order */
   std::vector<int> sorted_order(columns.size());
   std::transform(columns_with_data.begin(), columns_with_data.end(), sorted_order.begin(), [](column_pair pair){return pair.second;} );

   std::vector<Field> set = columns_with_data[0].first;
   for (auto column_entry : columns_with_data)
   {
      for (auto field : column_entry.first)
      {
         auto match = std::find_if(set.begin(), set.end(), [field](Field set_in_field){ return !set_in_field.is_equal(field); } );
         
         if (match != set.end())
         {
            set.push_back(field);
         }         
      }      
   }

   
   return 0;
}
