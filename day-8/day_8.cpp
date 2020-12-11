/*! \file day_7.cpp
*
*  \brief day-7 advent of code problem
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
#include <functional>
#include <set>
#include <map>


/*********************************** Consts ********************************************/

/************************************ Types ********************************************/


/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype **************************************/
/**
 * @brief enumeration of instructions
*/
enum class Instructions : unsigned
{
   nop = 0,
   acc,
   jmp
};


/**
 * @brief enumeration of possible emulation loop states
*/
enum class RunStatus : unsigned
{
   infinite_loop,
   terminated
};


/**
 * @brief map from string to instruction type
*/
static const std::map<std::string, Instructions> instruction_map{ {"nop", Instructions::nop}, {"acc", Instructions::acc}, {"jmp", Instructions::jmp} };


/**
 * @brief struct to store instructions
*/
struct Instruction
{
   Instructions instruction{Instructions::nop};
   int value{0};
   bool executed{false};
   Instruction( Instructions instruction, int value ) : instruction(instruction), value(value) {};
};


/**
 * @brief function to split a string based on a delimiter
 * @param input the string to split
 * @param substring substring to split by
 * @return 
*/
inline static std::vector<std::string> split(std::string &input, const std::string& substring)
{
   std::vector<std::string> list;
   size_t substring_pos{std::string::npos};
   while ((substring_pos = input.find(substring)) != std::string::npos)
   {
      list.push_back(input.substr(0, substring_pos));
      input.erase(0, substring_pos + substring.length());
   }
   list.push_back(input.substr(0, input.length()));
   return list;
}


class Emulator
{
   public:
      int accumulator{0};
      std::vector<Instruction> instructions;
   
      Emulator(const std::string& instruction_file)
      {
         this->load_instructions( instruction_file );
      }

      Emulator(std::vector<Instruction>& instructions_list) : instructions(instructions_list) 
      {
         for (auto& instruction : this->instructions)
         {
            instruction.executed = false;
         }
      };            

      /**
       * @brief 
       * @param instruction_file 
      */
      void load_instructions(const std::string& instruction_file)
      {
         std::ifstream stream{ instruction_file };
         std::string line;
         while (std::getline(stream, line))
         {
            auto strings = split(line, " ");
            Instruction instruction{instruction_map.at(strings[0]), std::stoi(strings[1]) };
            this->instructions.push_back(instruction);
         }
      }

      /**
       * @brief run the current instruction and return the next instruction
       * @param current_instruction 
       * @return 
      */
      int run_instruction(int current_instruction)
      {
         auto instruction = this->instructions[current_instruction];
         int next_instruction{0};
         switch (instruction.instruction)
         {
            case Instructions::acc:
               this->accumulator += instruction.value;
               next_instruction = current_instruction + 1;
               break;

            case Instructions::jmp:
               next_instruction = current_instruction + instruction.value;
               break;

            case Instructions::nop:
               next_instruction = current_instruction + 1;
               break;

            default:
               break;
         }
         this->instructions[current_instruction].executed = true;
         return next_instruction;
      }

      /**
       * @brief run the emulation terminating either after detecting an infinite loop or proper termination
      */
      RunStatus run(void)
      {         
         int current_instruction{0};
         RunStatus status{RunStatus::infinite_loop};

         while ( this->instructions[current_instruction].executed == false )
         {                                   
            int next_instruction = this->run_instruction(current_instruction);

            /* check for successful termination */
            if ( next_instruction == this->instructions.size() )
            {
               status = RunStatus::terminated;
               break;
            }

            current_instruction = next_instruction;            
         }
         return status;
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

   Emulator game{std::string{argv[1]}};
   auto status = game.run();
   std::cout << "value in the accumulator before executing an instruction twice: " << game.accumulator << std::endl;

   /* part two solution */      
   /* create a new vector of instructions that is a clone of the existing instruction set but with all nop/jmp swapped */
   const std::map <Instructions, Instructions> remap{ {Instructions::jmp, Instructions::nop}, 
                                                      {Instructions::nop, Instructions::jmp}, 
                                                      {Instructions::acc, Instructions::acc} };
   
   std::vector<Instruction> instructions;      
   std::transform( game.instructions.cbegin(), game.instructions.cend(), std::back_inserter(instructions), 
      [remap](Instruction &i)
      {
         i.instruction = remap.at(i.instruction);
         return i;
      }
   );

   /* simulate each game iteration by swapping the instructions around inside a lambda */
   auto run_simulation = [game, count = 0]( Instruction &i ) mutable
   {
      std::vector<Instruction> instructions = game.instructions;
      instructions[count] = i;

      Emulator new_emulator{instructions};
      RunStatus status = ( instructions[count].instruction == Instructions::acc ) ? RunStatus::infinite_loop : new_emulator.run();
      count++;
      return std::pair<RunStatus, int>{status, new_emulator.accumulator};
   };

   /* run each iteration and store the status + accumulator value in a std::pair */
   std::vector<std::pair<RunStatus, int>> run_status;
   std::transform( instructions.begin(), instructions.end(), std::back_inserter(run_status), run_simulation );

   /* fetch the successful swap and output the value */
   auto iterator = std::find_if(run_status.cbegin(), run_status.cend(), [](const auto &status){ return status.first == RunStatus::terminated; });   
   std::cout << "value in accumulator on successful exit: " << iterator->second << std::endl;
   return 0;
}
