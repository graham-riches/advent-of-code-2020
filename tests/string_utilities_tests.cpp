/*! \file string_utilities_tests.cpp
*
*  \brief tests for some string functions
* 
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include <string>
#include <gtest/gtest.h>
#include "string_utilities.h"

/**
 * @test that a string can be split by a single delimiter properly 
*/
TEST(string_utilities_tests, test_string_split_single_character)
{
   std::string test_string{"this,is,a,test,string"};
   std::vector<std::string> output = split(test_string, ",");
   EXPECT_EQ( "this", output[0] );
   EXPECT_EQ( 5, output.size() );
}

/**
 * @test that a string can be split by a multi-character delimiter properly
*/
TEST(string_utilities_tests, test_string_split_multiple_characters)
{
   std::string test_string{ "this,,is,,a,,test,,string" };
   std::vector<std::string> output = split(test_string, ",,");
   EXPECT_EQ("this", output[0]);
   EXPECT_EQ(5, output.size());
}

/**
 * @test that a delimiter longer than the string returns a length one vector with just the string in it 
*/
TEST(string_utilities_tests, test_delimiter_longer_than_string_returns_string)
{  
   std::string test_string{"hello"};
   std::string delimiter{"worlds"};
   std::vector<std::string> output = split(test_string, delimiter);
   EXPECT_EQ("hello", output[0]);
   EXPECT_EQ(1, output.size());
}

/**
 * @test stripping whitespace from a string 
*/
TEST(string_utilities_tests, test_string_strip_whitespace)
{
   std::string test_string{"string with whitespace"};
   test_string = strip(test_string, " " );
   EXPECT_EQ("stringwithwhitespace", test_string);
}