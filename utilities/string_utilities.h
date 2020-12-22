/*! \file string_utilities.h
*
*  \brief a set of useful string functions I've needed many times
* 
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include <fstream>
#include <iostream>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

/****************************** Function Definitions ***********************************/
/**
 * @brief convert a filename to an open filestream
 * @param filename
 * @return
*/
std::ifstream open_file( const std::string &filename ) {
    return std::ifstream{ filename };
}

/**
 * @brief read from an input stream into a vector of lines
 * @param stream
 * @return
*/
std::vector<std::string> read_file( std::ifstream &&stream ) {
    std::vector<std::string> data;
    std::string line;
    while ( std::getline( stream, line ) ) {
        data.push_back( line );
    }
    return data;
}

/**
 * @brief split a string into a vector of strings
 * @param input the string to split
 * @param substring substring delimiter
 * @return 
*/
std::vector<std::string> split( std::string &input, const std::string &delimiter ) {
    std::vector<std::string> list;
    size_t substring_pos = std::string::npos;
    while ( ( substring_pos = input.find( delimiter ) ) != std::string::npos ) {
        list.push_back( input.substr( 0, substring_pos ) );
        input.erase( 0, substring_pos + delimiter.length( ) );
    }
    list.push_back( input.substr( 0, input.length( ) ) );
    return list;
}

/**
 * @brief strip a delimiter out of a string
 * @param input 
 * @param substring 
 * @return 
*/
inline std::string &strip( std::string &input, const std::string &substring ) {
    size_t substring_pos = std::string::npos;
    while ( ( substring_pos = input.find( substring ) ) != std::string::npos ) {
        input.erase( substring_pos, substring.length( ) );
    }
    return input;
}