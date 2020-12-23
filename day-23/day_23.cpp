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

/********************************** Functions *******************************************/

class CrabbyCupGame {
  public:
    CrabbyCupGame( CircularList<int> list )
        : list( list )
        , min_value( list.get_minimum( ) )
        , max_value( list.get_maximum( ) ){ };

    /**
     * @brief play the game for a set number of rounds
     * @param rounds how many rounds to play
     * @param starting_cup cup to start with
     * @return the circular list object
    */
    CircularList<int> play_game( int rounds, int starting_cup ){
        int active_cup = starting_cup;
        while (rounds--){
            active_cup = play_round(active_cup);            
        }
        return list;
    }

  private:
    CircularList<int> list;
    int min_value;
    int max_value;

    /**
    * @brief get the destination cup value
    * @param round_cups the current cups in play
    * @param active cup the currently active main cup
    * @return int id
    */
    int get_destination_cup( std::vector<int> round_cups, int active_cup ) {
        int target_id = active_cup - 1;

        while ( true ) {
            target_id = ( target_id < min_value ) ? max_value : target_id;
            auto count = std::count( round_cups.cbegin( ), round_cups.cend( ), target_id );
            if ( count == 0 ) {
                break;
            } else {
                target_id--;
            }
        }
        return target_id;
    }

    /**
    * @brief play a round
    * @param active_cup the active cup for the round
    * @return next cup
    */
    int play_round( int active_cup ) {
        auto cups = list.remove_after_element( active_cup, 3 );
        int destination = get_destination_cup( cups, active_cup );
        list.insert_after_element(destination, cups);        
        return list.get_next(active_cup);
    }
};

/**
 * @brief main application entry point
*/
int main( int64_t argc, char *argv[] ) {
    auto start = std::chrono::steady_clock::now();

    /*------------------------------ Part One Solution ------------------------------*/
    std::vector<int> input{ 5, 9, 8, 1, 6, 2, 7, 3, 4 };
    CircularList list{ input };
    CrabbyCupGame game_one{ list };
    auto result = game_one.play_game(100, input[0]);
    result.print_from_element(1);

    /*------------------------------ Part Two Solution ------------------------------*/
    std::vector<int> more_cups(1000000 - input.size());
    std::iota(more_cups.begin(), more_cups.end(), *std::max_element(input.begin(), input.end()) + 1 );
    input.insert(input.end(), more_cups.begin(), more_cups.end());
    CircularList new_list{input};
    CrabbyCupGame game_two{new_list};
    auto round_two_result = game_two.play_game( 10000000, input[0] );
    auto multiple_one = round_two_result.get_next(1);
    auto multiple_two = round_two_result.get_next(multiple_one);

    int64_t multiple = static_cast<int64_t>(multiple_one) * static_cast<int64_t>(multiple_two);
    std::cout << "Multiple is: " << multiple << "\n";
    
    auto end = std::chrono::steady_clock::now();

    std::cout << "Elapsed Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds \n";

    return 0;
}
