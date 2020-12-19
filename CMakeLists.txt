################################################
# \file CmakeLists.txt
# \author Graham Riches
# \brief build for the advent of code projects
################################################
cmake_minimum_required(VERSION 3.15.3)
project(advent_of_code)


# set the language versions
enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


# set MSVC compiler flags and linker flags
set(CMAKE_CXX_FLAGS_RELEASE "/MT")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi /Ob0 /Od /RTC1")
set(MSVC_LINK_FLAGS "/DEBUG")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${MSVC_LINK_FLAGS}")



# find any packages
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS}
					${CMAKE_SOURCE_DIR}/utilities
					${CMAKE_SOURCE_DIR}/lib/googletest/googletest/include
)


# add subdirectories
add_subdirectory(lib/googletest)
add_subdirectory(tests)
add_subdirectory(day-1)
add_subdirectory(day-2)
add_subdirectory(day-3)
add_subdirectory(day-4)
add_subdirectory(day-5)
add_subdirectory(day-7)
add_subdirectory(day-8)
add_subdirectory(day-9)
add_subdirectory(day-10)
add_subdirectory(day-11)
add_subdirectory(day-12)
add_subdirectory(day-13)
add_subdirectory(day-14)
add_subdirectory(day-16)
add_subdirectory(day-17)
add_subdirectory(day-18)