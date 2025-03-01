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

# Set the unified test directory
set(TEST_DIR test)

# Test executables
add_executable(expr_test ${TEST_DIR}/test_constant_expr.c)
target_link_libraries(expr_test ptx_ast)
install(TARGETS expr_test DESTINATION bin)

add_executable(prelude_test ${TEST_DIR}/test_prelude.c)
target_link_libraries(prelude_test prelude)
install(TARGETS prelude_test DESTINATION bin)

add_executable(initializer_array_test ${TEST_DIR}/test_initializer_array.c)
target_link_libraries(initializer_array_test prelude)
install(TARGETS initializer_array_test DESTINATION bin)

# Test for initializer helpers
add_executable(initializer_test ${TEST_DIR}/test_initializer.c)
target_link_libraries(initializer_test prelude ptx_ast)
target_include_directories(initializer_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS initializer_test DESTINATION bin)

# Test for constant parsing
add_executable(constant_parse_test ${TEST_DIR}/test_constant_parse.c)
target_link_libraries(constant_parse_test ptx_parse)
target_include_directories(constant_parse_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS constant_parse_test DESTINATION bin)

# Additional useful tests from the test directory
add_executable(ptx_type_test ${TEST_DIR}/test_ptx_type.c)
target_link_libraries(ptx_type_test ptx_ast)
target_include_directories(ptx_type_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS ptx_type_test DESTINATION bin)

add_executable(ptx_packed_float_test ${TEST_DIR}/test_ptx_packed_float.c)
target_link_libraries(ptx_packed_float_test ptx_ast)
target_include_directories(ptx_packed_float_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS ptx_packed_float_test DESTINATION bin)

# Tests from tests directory that seemed useful
add_executable(graphics_types_test ${TEST_DIR}/test_graphics_types.c)
target_link_libraries(graphics_types_test ptx_ast)
target_include_directories(graphics_types_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS graphics_types_test DESTINATION bin)

add_executable(ast_test ${TEST_DIR}/test_ast.c)
target_link_libraries(ast_test ptx_ast ptx_parse)
target_include_directories(ast_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include ${CMAKE_SOURCE_DIR}/ptx_parse/include)
install(TARGETS ast_test DESTINATION bin)
