/*! \file day_23.cpp
*
*  \brief day-23 advent of code problem
* 
*
*  \author Graham Riches
*  \details circular cup game
*/

/********************************** Includes *******************************************/
#include "circular_list.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <set>
#include <sstream>
#include <vector>
#include <list>

/********************************** Functions *******************************************/

std::list<int> get_next_entries( std::list<int> &list, int current_id, int count ) {
    auto it = std::find(list.begin(), list.end(), current_id);
    auto index = std::next(it, count);
    std::list<int> entries(count);
    /*
    if ( index + count > list.size() ) {
        
    }
    */
    



    //list.erase(index, index + count);
    //return entries;
    return { };
}



/**
 * @brief main application entry point
*/
int main( int64_t argc, char *argv[] ) {
    std::vector<int> input{3, 8, 9, 1, 2, 5, 4, 6, 7};
    //auto test = get_next_entries( input, 8, 3 );
    
    CircularList test{input};


    return 0;
}
