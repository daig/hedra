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

# Test executables
add_executable(expr_test tests/test_constant_expr.c)
target_link_libraries(expr_test ptx_ast)
install(TARGETS expr_test DESTINATION bin)

add_executable(prelude_test tests/test_prelude.c)
target_link_libraries(prelude_test prelude)
install(TARGETS prelude_test DESTINATION bin)

add_executable(initializer_array_test tests/test_initializer_array.c)
target_link_libraries(initializer_array_test prelude)
install(TARGETS initializer_array_test DESTINATION bin)

# Test for initializer helpers
add_executable(initializer_test tests/test_initializer.c)
target_link_libraries(initializer_test prelude ptx_ast)
target_include_directories(initializer_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS initializer_test DESTINATION bin)

# Test for constant parsing
add_executable(constant_parse_test tests/test_constant_parse.c)
target_link_libraries(constant_parse_test ptx_parse)
target_include_directories(constant_parse_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS constant_parse_test DESTINATION bin)
