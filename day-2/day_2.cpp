/*! \file day_2.cpp
*
*  \brief day-2 advent of code problem
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <vector>

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * @brief password object
*/
class PasswordWithPolicy
{
   private:
   char policy_letter;
   int min_policy_count;
   int max_policy_count;
   std::string password;

   PasswordWithPolicy( char policy_letter, int min_count, int max_count, const std::string &password )
      : policy_letter( policy_letter )
      , min_policy_count( min_count )
      , max_policy_count( max_count )
      , password( password )
   { }

   public:
   /**
    * @brief factory method to create a new password object from a string
    * @param input the input string
    * @return new password object
    * @note there is absolutely no error checking here, and some dubious hard-coding
   */
   static PasswordWithPolicy from_string( const std::string &input )
   {
      std::basic_regex password_policy{ "(\\d+)-(\\d+) ([a-z]): ([a-z]+)", std::regex::icase };
      std::smatch matches;
      std::regex_search( input, matches, password_policy );

      PasswordWithPolicy new_password( std::string( matches[3] )[0], std::stoi( matches[1] ), std::stoi( matches[2] ), std::string( matches[4] ) );
      return new_password;
   }

   /**
    * @brief check if the password contains the correct number of     
    * @return true/false
   */
   bool is_valid( void ) const
   {
      auto count = std::count( this->password.cbegin( ), this->password.cend( ), this->policy_letter );
      return ( ( count >= this->min_policy_count ) && ( count <= this->max_policy_count ) );
   }

   /**
    * @brief checks if a password is valid based on new arbitrary rules
    * @return true/false
   */
   bool is_valid_from_new_arbitrary_rules( void ) const
   {
      auto i = min_policy_count - 1;
      auto j = max_policy_count - 1;
      bool valid = false;
      if ( password[i] == policy_letter )
      {
         valid = ( password[j] != policy_letter );
      }
      else if ( password[j] == policy_letter )
      {
         valid = ( password[i] != policy_letter );
      }
      return valid;
   }
};

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
 * @brief main application entry point
 * @param argc number of arguments
 * @param argv pointer to array of inputs
 * @return integer return code
*/
int main( int argc, char *argv[] )
{
   /* read the input stream into a vector of password objects */
   std::vector<PasswordWithPolicy> passwords;

   std::ifstream password_file{ std::string{ argv[1] } };
   std::string line;
   while ( std::getline( password_file, line ) )
   {
      passwords.push_back( PasswordWithPolicy::from_string( line ) );
   }


   /* part 1 solution */
   auto valid_passwords_part_1 = std::transform_reduce(
      passwords.cbegin( ), passwords.cend( ), 0L, std::plus<>( ), []( const PasswordWithPolicy &pass ) { return pass.is_valid( ); } );
   std::cout << "Number of passwords valid from initial rules: " << valid_passwords_part_1 << std::endl;


   /* part 2 solution */
   auto valid_passwords_part_2 = std::transform_reduce(
      passwords.cbegin( ), passwords.cend( ), 0L, std::plus<>( ), []( const PasswordWithPolicy &pass ) { return pass.is_valid_from_new_arbitrary_rules( ); } );
   std::cout << "Number of valid passwords from aribitrary rules: " << valid_passwords_part_2 << std::endl;

   return 0;
}

