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

# Add the PTX parse tool executable
add_executable(ptx_parse_tool ptx_parse_tool.c)
target_link_libraries(ptx_parse_tool ptx_parse ptx_print ptx_ast prelude)
target_include_directories(ptx_parse_tool PRIVATE 
    ${CMAKE_SOURCE_DIR}/ptx_ast/include
    ${CMAKE_SOURCE_DIR}/ptx_parse/include
    ${CMAKE_SOURCE_DIR}/ptx_print/include
    ${CMAKE_SOURCE_DIR}/prelude/include
)
install(TARGETS ptx_parse_tool DESTINATION bin)
