cmake_minimum_required(VERSION 3.15.3)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_SYSTEM_NAME Generic)

project(advent_of_code)

enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_COMPILER_IS_GNUCXX)
	message(STATUS "GCC Compiler detected, adding flags")
	set(CMAKE_CXX_FLAGS, "${CMAKE_CXX_FLAGS} -Wall -Wextra -O0 -g3")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(MSVC_LINK_FLAGS "/DEBUG")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${MSVC_LINK_FLAGS}")


add_subdirectory(day-1)
add_subdirectory(day-2)
add_subdirectory(day-3)
add_subdirectory(day-4)
add_subdirectory(day-5)
add_subdirectory(day-7)
