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

#include "better_string.h"

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
struct BagNode
{
   BetterString name;
   std::vector<BetterString> children;
   std::vector<int> child_bag_count;

   BagNode(BetterString& name, std::vector<BetterString>& children)
   {
      this->name = name;      
      for (auto &child : children)
      {
         std::basic_regex match_digits{"(\\d+ )", std::regex::icase };
         std::smatch matches;
         std::regex_search(child.data, matches, match_digits);         
         if (matches.size() == 2)
         {                        
            this->child_bag_count.push_back(std::stoi(std::string{ matches[1] }));
            std::string match{matches[0]};
            child.data.erase(0, child.data.find(match) + match.size());
            this->children.push_back(child);
         }
      }
      return;
   }

   /**
    * @brief check if a string is contained as a child in a node
    * @param child the child name
    * @return bool
   */
   bool contains_as_child(const std::string& child) const
   {
      return std::any_of( cbegin(this->children), cend(this->children), [child](const BetterString &p){ return p.data.find( child ) != std::string::npos; } );
   }

   /**
    * @brief check if a string is contained as the parent name in a node
    * @param parent the child name
    * @return bool
   */
   bool contains_as_parent(const std::string& parent) const
   {
      return (this->name.data.find(parent) != std::string::npos);
   }

   friend bool operator <(const BagNode& a, const BagNode& b)
   {
      return a.name.data < b.name.data;
   }
};

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype **************************************/
/**
 * @brief template function to open a file stream, apply a function to each line, and store the results in a vector
 * @param filepath path to the file stream
 * @param callable function to create a BagNode object from the input file stream
 * @return vector of T
*/
std::vector<BagNode> file_input_to_vector(const std::string& filepath)
{
   std::vector<BagNode> data;
   std::ifstream stream{ filepath };
   std::string line;
   while (std::getline(stream, line))
   {
      BetterString input{ line };
      auto strings = input.strip(" bags").strip(" bag").strip(".").split(" contain");
      auto child = strings[0];
      auto parents = strings[1].split(", ");
      BagNode node{ child, parents };
      data.push_back(node);
   }
   return data;
}

std::vector<BagNode> get_children(std::vector<BagNode>& all_bags, const BagNode& bag)
{
   std::vector<BagNode> bags;
   for (auto& child : bag.children)
   {
      std::copy_if(cbegin(all_bags), cend(all_bags), std::back_inserter(bags),
         [child]( const BagNode &b){ return b.name.data == child.data; }
      );
   }         
   return bags;
}

/**
 * @brief traverse a bag and get the multiplication of it's unique children (recursive)
 * @param bag 
 * @return 
*/
static int search_bags(std::vector<BagNode>&all_bags, const BagNode& bag)
{
   int count = 1;
   std::vector<BagNode> child_nodes = get_children( all_bags, bag );   
   
   for (int i = 0; i < child_nodes.size(); i++ )
   {
      int multiplier = ( child_nodes[i].children.size() > 0 ) ? search_bags(all_bags, child_nodes[i]) : 1;
      count += bag.child_bag_count[i] * multiplier;
   }

   return count;
}



/****************************** Functions Definition ***********************************/
/**
 * @brief main application entry point
 * @param argc number of arguments
 * @param argv pointer to array of inputs
 * @return integer return code
*/
int main( int argc, char *argv[] )
{     
   std::vector<BagNode> results = file_input_to_vector( std::string{ argv[1] } );


   /* part one solution */
   size_t bag_count{0};
   std::vector<BagNode> new_parents{ BagNode{BetterString{"shiny gold"}, std::vector<BetterString>()} };
   std::set<BagNode> unique_bags{ new_parents[0] };
   
   do
   {
      bag_count = unique_bags.size();
      std::vector<BagNode> bags;
      for (auto bag : new_parents)
      {                  
         auto search = [bag](const BagNode& node) { return node.contains_as_child(bag.name.data); };
         std::copy_if(cbegin(results), cend(results), std::back_inserter(bags), search);
      }
      /* clear out the existing parents */
      new_parents.clear();

      /* append unique new values to the existing set, and reset the new parents to search for the next iteration */
      for (auto bag : bags)
      {
         auto result = unique_bags.insert(bag); //!< returns a std::pair<iterator, bool>
         if (result.second)
         {
            new_parents.push_back(bag);
         }
      }      
   }
   while ( unique_bags.size() != bag_count );
  
   std::cout << "Total containing bags: " << unique_bags.size() - 1; //!< subtract the original gold bag

   /* part two solution - get the total number of bags contained inside a shiny gold bag */
   auto start = std::find_if(cbegin(results), cend(results), [](const BagNode& node){ return node.contains_as_parent(std::string{"shiny gold"});});

   /* take each bag in children set, and start a new tree structure based on those */
   auto total_bags = search_bags( results, *start ) - 1; 
   std::cout << "Total bags to buy: " << total_bags << std::endl;

   return 0;
}
