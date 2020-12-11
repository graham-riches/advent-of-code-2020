/*! \file better_string.h
*
*  \brief modified string class with some nice syntactic sugar for chaining string operations
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include <ostream>
#include <string>
#include <vector>

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * @brief a wrapper class on std::string that allows chaining common operations together
*/
class BetterString
{
public:
   std::string data;

   BetterString(std::string&& s)
   {
      this->data = std::move(s);
   }

   BetterString(std::string& s)
   {
      this->data = s;
   }

   BetterString(){};

   /**
    * @brief stript a substring out of the string
    * @param substring the substring to split
    * @return reference to self
   */
   inline BetterString& strip(const std::string& substring)
   {
      size_t substring_pos = std::string::npos;
      while ((substring_pos = this->data.find(substring)) != std::string::npos)
      {
         this->data.erase(substring_pos, substring.length());
      }
      return *this;
   }

   /**
    * @brief split a string object into sub strings based on a delimiter string
    * @param input the input string object
    * @param substring to split by
    * @return vector of strings
   */
   inline std::vector<BetterString> split(const std::string& substring)
   {
      std::vector<BetterString> list;
      size_t substring_pos = std::string::npos;
      while ((substring_pos = this->data.find(substring)) != std::string::npos)
      {
         list.push_back(this->data.substr(0, substring_pos));
         this->data.erase(0, substring_pos + substring.length());
      }
      list.push_back(this->data.substr(0, this->data.length()));
      return list;
   }

   /**
    * @brief ostream operator for the string
    * @param os output stream
    * @param input the string object
    * @return the stream
   */
   friend std::ostream& operator<<(std::ostream& os, const BetterString& input)
   {
      os << input.data;
      return os;
   }
};


