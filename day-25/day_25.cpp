/*! \file day_25.cpp
*
*  \brief day-25 advent of code problem
* 
*
*  \author Graham Riches
*  \details cryptography puzzle
*/

/********************************** Includes *******************************************/
#include <algorithm>
#include <chrono>
#include <iostream>
#include <cstdint>

/********************************** Functions *******************************************/

/**
 * @brief main application entry point
*/
int main( int argc, char *argv[] ) {
    auto start = std::chrono::steady_clock::now();
    
    uint64_t door_key{11404017};
    uint64_t card_key{13768789};

    uint64_t encryption_key{1};
    uint64_t temp_key{1};
    while (temp_key != card_key){
        temp_key = ( temp_key * 7 ) % 20201227;
        encryption_key = ( encryption_key * door_key ) % 20201227;
    }

    std::cout << "Encryption key: " << encryption_key << "\n";

    auto end = std::chrono::steady_clock::now();
    std::cout << "Elapsed Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds \n";

    return 0;
}
