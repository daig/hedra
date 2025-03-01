cmake_minimum_required(VERSION 3.10)

# Project name and version
project(PTX_PROJECT VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add subdirectories for each library
add_subdirectory(prelude)
add_subdirectory(ptx_ast)
add_subdirectory(ptx_parse)
add_subdirectory(ptx_print)

# Add test directory
add_subdirectory(test)
