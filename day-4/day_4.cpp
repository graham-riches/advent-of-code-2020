/*! \file day_4.cpp
*
*  \brief day-4 advent of code problem
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
#include <map>

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
enum class PassportFields : unsigned
{
   id,
   country_id,
   birth_year,
   issue_year,
   expiration_year,
   height,
   hair_color,
   eye_color
};

static const std::map<PassportFields, std::string> passport_fields_regex_map =
{
   { PassportFields::id, "pid:#?([A-Za-z0-9]+)" }, { PassportFields::country_id, "cid:#?([A-Za-z0-9]+)" }, { PassportFields::birth_year, "byr:#?([A-Za-z0-9]+)" },
   { PassportFields::issue_year, "iyr:#?([A-Za-z0-9]+)" }, { PassportFields::expiration_year, "eyr:#?([A-Za-z0-9]+)" }, { PassportFields::height, "hgt:#?([A-Za-z0-9]+)"},
   { PassportFields::hair_color, "hcl:#?([A-Za-z0-9]+)" }, { PassportFields::eye_color, "ecl:#?([A-Za-z0-9]+)" }
};

static const std::map<PassportFields, std::string> password_validation_regex =
{
   { PassportFields::id, "pid:([0-9]{9})[\\s\\n]" }, { PassportFields::country_id, "cid:#?([A-Za-z0-9]+)" }, { PassportFields::birth_year, "byr:([0-9]{4})[\\s\\n]" },
   { PassportFields::issue_year, "iyr:([0-9]{4})[\\s\\n]" }, { PassportFields::expiration_year, "eyr:([0-9]{4})[\\s\\n]" }, { PassportFields::height, "hgt:([0-9]+)[a-z]{2}[\\s\\n]"},
   { PassportFields::hair_color, "hcl:([A-Za-z0-9]{6})[\\s\\n]" }, { PassportFields::eye_color, "ecl:([a-z]{3})[\\s\\n]" }
};


class Passport
{
private:
   std::map<PassportFields, std::string> fields;
   Passport() {};

public:   
   /**
    * @brief factory method to create a passport object from a string
    * @param input 
    * @return 
   */
   static Passport from_string(const std::string& input, std::map<PassportFields, std::string> regex_map)
   {
      Passport new_passport;
      
      /* iterate over the regex keys */
      for (auto& [key, regex_query] : regex_map)
      {
         new_passport.fields[key] = from_regex( input, regex_query );
      }
      return new_passport;
   }

   /**
   * @brief return the string matching a regex query or empty otherwise
   * @param input string input
   * @param regex the regex query
   * @return the string result
   */
   static std::string from_regex(const std::string& input, const std::string& regex)
   {
      std::smatch matches;
      std::regex_search(input, matches, std::basic_regex{ regex, std::regex::icase });

      std::string result = ( matches.size() == 2 ) ? std::string(matches[1]) : "";
      return result;
   }

   /**
    * @brief check that a passport object contains all of the fields present in an input list
    * @param check_fields the fields to check
    * @return true if all fields are non-null
   */
   bool check_contains(std::vector<PassportFields> check_fields) const
   {
      std::vector<bool> contains;
      std::transform( cbegin(check_fields), cend(check_fields), std::back_inserter(contains), [this](PassportFields field){ return (this->fields.at(field) != "");} );
      auto result = std::all_of( cbegin(contains), cend(contains), [](bool value){ return value;} );
      return result;
   }

   /**
    * @brief check if a passport is valid based on the criteria
    * @return true/false
    * @note using a heinous chunk of terrible logic for this :D
   */
   bool validate_all_fields( void ) const
   {   
      /* check the years and the eye color */
      int birth_year = (this->fields.at(PassportFields::birth_year) != "") ? std::stoi(this->fields.at(PassportFields::birth_year)) : 0;
      int issue_year = (this->fields.at(PassportFields::issue_year) != "") ? std::stoi(this->fields.at(PassportFields::issue_year)) : 0;
      int expiration_year = (this->fields.at(PassportFields::expiration_year) != "") ? std::stoi(this->fields.at(PassportFields::expiration_year)) : 0;
      std::vector<std::string> valid_eye_colors{ "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
      
      std::vector<bool> valid(4);
      valid[0] = ((birth_year >= 1920) && (birth_year <= 2002));
      valid[1] = ((issue_year >= 2010) && (issue_year <= 2020));
      valid[2] = ((expiration_year >= 2020) && (expiration_year <= 2030));
      valid[3] = std::find(cbegin(valid_eye_colors), cend(valid_eye_colors), this->fields.at(PassportFields::eye_color) ) != std::end(valid_eye_colors);
      auto result = std::all_of(cbegin(valid), cend(valid), [](bool value) { return value; });
      return result;
   }

   friend std::ostream& operator << (std::ostream& os, const Passport& pass)
   {
      for (auto& [key, value] : pass.fields)
      {
         os << passport_fields_regex_map.at(key) << " : " << value << "\n";
      }
      os << "\n";
      return os;
   }

};


/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/
std::vector<std::string> split_by_string( std::string &input, const std::string &delimiter );


/****************************** Functions Definition ***********************************/
/**
 * @brief main application entry point
 * @param argc number of arguments
 * @param argv pointer to array of inputs
 * @return integer return code
*/
int main( int argc, char *argv[] )
{
   std::ifstream passport_data_stream{ std::string{ argv[1] } };
   /* read raw string input */
   std::string file_data{ std::istreambuf_iterator<char>(passport_data_stream), std::istreambuf_iterator<char>( ) };
      
   /* chunk the string into a vector of strings of each passports information */
   std::vector<std::string> passport_data = split_by_string( file_data, "\n\n" );

   /* strip the endlines from each string instance */
   std::transform( begin(passport_data), end(passport_data), begin(passport_data), 
                   [](std::string &input) { std::replace(input.begin(), input.end(), '\n', ' '); return input; }
                  );

   /* part one solution */

   /* de-serialize the passport strings into a vector of password containers */
   std::vector<Passport> passports;
   std::transform( cbegin(passport_data), cend(passport_data), std::back_inserter(passports),
                   [](const std::string &input ){ return Passport::from_string( input, passport_fields_regex_map); } );

   /* transform reduce the passports based on a validation criteria and return the sum */
   std::vector<PassportFields> check_fields{ PassportFields::id, PassportFields::birth_year, PassportFields::issue_year,
                                             PassportFields::expiration_year, PassportFields::height, PassportFields::hair_color, PassportFields::eye_color };

   auto val = std::transform_reduce( cbegin(passports), cend(passports), 0L,
                                                 std::plus<>(),
                                                 [check_fields]( const Passport &pass ){ return pass.check_contains(check_fields);}
                                                );

   std::cout << "number of valid passports: " << val << std::endl;


   /* part two solution */
   std::vector<Passport> raw_passports;
   std::transform(cbegin(passport_data), cend(passport_data), std::back_inserter(raw_passports),
      [](const std::string& input) { return Passport::from_string(input, password_validation_regex); });

   auto val1 = std::transform_reduce(cbegin(raw_passports), cend(raw_passports), 0L,
      std::plus<>(),
      [](const Passport& pass) { return pass.validate_all_fields(); }
   );
   std::cout << "number of valid passports: " << val1 << std::endl;

   return 0;
}


/**
 * @brief chunk an input string into a vector of substrings based on a string delimiter
 * @param input the input string
 * @param delimiter the character sequence
 * @return 
*/
std::vector<std::string> split_by_string( std::string& input, const std::string& delimiter )
{
   std::vector<std::string> sub_strings;
   size_t delimiter_position = input.find(delimiter);
   while (delimiter_position != std::string::npos)
   {
      sub_strings.push_back( input.substr(0, delimiter_position));
      input.erase(0, delimiter_position + delimiter.length());
      delimiter_position = input.find(delimiter);
   }
   /* handle the edge case of the last value */
   sub_strings.push_back(input.substr(0, delimiter_position));
   return sub_strings;
}


