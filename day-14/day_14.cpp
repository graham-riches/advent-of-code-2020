/*! \file day_14.cpp
*
*  \brief day-14 advent of code problem
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
#include <functional>

/************************************ Types ********************************************/
enum class Instruction : unsigned
{
   write_memory = 0,
   update_bitmask
};

enum class DecoderVersion : unsigned
{
   one = 0,
   two,
};


class Computer
{
   private:
      const std::map<Instruction, std::regex> regex_queries{ {Instruction::write_memory,   std::regex{ "mem\\[(\\d+)\\] = (\\d+)", std::regex::icase }}, 
                                                             {Instruction::update_bitmask, std::regex{ "mask = ([01X]+)", std::regex::icase        }}};
      
   public:
      std::string bitmask;
      std::map<int64_t, int64_t> memory;
      std::vector<std::string> program;
      DecoderVersion decoder_mode;

      Computer( const std::string &program_input, DecoderVersion version ) 
         : decoder_mode(version)
      {
         std::ifstream stream{ program_input };
         std::string line;         
         while (std::getline(stream, line))
         {
            this->program.push_back(line);
         }         
      };

      /**
       * @brief run the computers main boot program
      */
      int64_t run(void)
      {                           
         std::for_each(this->program.begin(), this->program.end(), [this](const std::string &a){ this->decode_and_run_instruction(a);} );

         /* sum the memory */
         int64_t memory_sum{0};
         for (auto& [address, value] : this->memory)
         {
            memory_sum += value;
         }

         return memory_sum;
      }

      /**
       * @brief parse the string input and dispatch the corresponding instruction
       * @param instruction the instruction string
      */
      void decode_and_run_instruction(const std::string& instruction)
      {
         for (auto& [key, query] : this->regex_queries)
         {
            std::smatch matches;
            std::regex_search( instruction, matches, query);

            if (matches.size() != 0)
            {
               switch (key)
               {
                  case Instruction::update_bitmask:
                     this->update_bitmask( std::string{matches[1]} );
                     break;

                  case Instruction::write_memory:
                     switch (this->decoder_mode)
                     {
                        case DecoderVersion::one:
                           this->apply_version_one_rules(std::stoll(matches[1]), std::stoll(matches[2]));
                           break;
                        
                        case DecoderVersion::two:
                           this->apply_version_two_rules(std::stoll(matches[1]), std::stoll(matches[2]));
                           break;
                        
                        default:
                           break;
                     }                     
                     break;

                  default:
                     break;
               }
            }
         }
      }

      /**
       * @brief handle an instruction to update the current bitmask
       * @param mask 
      */
      inline void update_bitmask(const std::string& mask)
      {
         this->bitmask = mask;
         std::reverse(this->bitmask.begin(), this->bitmask.end());
      }

      /**
       * @brief convert an integer value into a boolean array of bits
       * @param value the value to convert
       * @return boolean array
      */
      inline std::vector<bool> value_to_bits(const int64_t& value)
      {
         std::vector<bool> bits(64);
         for (int i = 0; i < bits.size(); i++)
         {
            auto temp = value & ( 0x01LL << i);
            bits[i] = static_cast<bool>( temp );
         }
         return bits;
      }

      /**
       * @brief convert a boolean array of bits into an int64
       * @param bits the array of bits
       * @return int value
      */
      inline int64_t bits_to_value(std::vector<bool>& bits)
      {
         int64_t value{0};
         
         for (int i = 0; i < bits.size(); i++)
         {
            value |= static_cast<int64_t>( bits[i]) << i;
         }
         return value;
      }

      /**
       * @brief apply the first part of the solution bitmask operator and save it to memory
       * @param value 
       * @return 
      */
      void apply_version_one_rules(const int64_t& memory_address, const int64_t& value)
      {
         /* convert the value into a bit array */
         auto bits = this->value_to_bits(value);

         /* apply the bitmask */
         int offset{ 0 };
         for (auto& mask : this->bitmask)
         {
            bits[offset] = (mask == 'X') ? bits[offset] : std::stoi(std::string{ mask });
            offset++;
         }

         this->write_to_memory( memory_address, this->bits_to_value(bits) );         
      }

      /**
       * @brief apply the rules for part two of the solution
       * @param memory_address 
       * @param value 
      */
      void apply_version_two_rules(const int64_t& memory_address, const int64_t& value)
      {
         /* convert the default addres into a vector of bits */
         std::vector<bool> bits = this->value_to_bits( memory_address );

         /* get the number of combinations */
         auto floating_bit_count = std::count_if(this->bitmask.cbegin(), this->bitmask.cend(), [](const char c) { return (c == 'X'); });
         const int64_t total_addresses = std::pow(2, floating_bit_count);
         
         for (int combination = 0; combination < total_addresses; combination++)
         {
            std::vector<bool> new_address = bits;
            int bit_offset{0};
            
            for ( int i = 0; i < this->bitmask.size(); i++ )
            {
               if (this->bitmask[i] == '1')
               {
                  new_address[i] = true;
               }
               else if (this->bitmask[i] == 'X')
               {
                  new_address[i] = ( combination >> bit_offset ) & 1;
                  bit_offset++;
               }               
            }
            auto temp = this->bits_to_value(new_address);
            this->write_to_memory(this->bits_to_value(new_address), value);
         }
      }


      /**
       * @brief write a value to memory
       * @param memory_address address to save to
       * @param value value to save pre-masking
      */
      void write_to_memory(const int64_t& memory_address, const int64_t& value)
      {                  
         if ( this->memory.find(memory_address) == this->memory.end() )
         {
            this->memory.insert({ memory_address, value });
         }
         else
         {
            this->memory.at(memory_address) = value;
         }         
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
   Computer version_one_computer{std::string{argv[1]}, DecoderVersion::one };
   auto memory_sum = version_one_computer.run();
   std::cout << "Sum of all memory is " << memory_sum << "\n";


   Computer version_two_computer{ std::string{argv[1]}, DecoderVersion::two };
   auto memory_sum_2 = version_two_computer.run();
   std::cout << "Sum of all memory is " << memory_sum_2 << "\n";

   return 0;
}
