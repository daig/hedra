cmake_minimum_required(VERSION 3.10)

# Project name and version
project(PTX_AST VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
set(AST_SOURCES
    src/ptx_predefined_identifier.c
    src/ptx_identifier.c
    src/ptx_constant.c
    src/ptx_constant_expr.c
    src/ptx_type_fundamental.c
    src/ptx_alternate_float_format.c
    src/ptx_type_packed_float.c
    src/ptx_type.c
    src/ptx_scalar_float_type.c
)

# Header files (not required for compilation, but good to list for IDE integration)
set(AST_HEADERS
    include/prelude.h
    include/ptx_predefined_identifier.h
    include/ptx_identifier.h
    include/ptx_constant.h
    include/ptx_constant_expr.h
    include/ptx_instruction.h
    include/ptx_type_fundamental.h
    include/ptx_alternate_float_format.h
    include/ptx_type_packed_float.h
    include/ptx_type.h
    include/ptx_scalar_float_type.h
)

# Create a library
add_library(ptx_ast STATIC ${AST_SOURCES})

# Install targets
install(TARGETS ptx_ast DESTINATION lib)
install(FILES ${AST_HEADERS} DESTINATION include)

# Test executable
add_executable(ast_test tests/test_ast.c)
target_link_libraries(ast_test ptx_ast)
install(TARGETS ast_test DESTINATION bin)

# Test executable for constant expressions
add_executable(expr_test tests/test_constant_expr.c)
target_link_libraries(expr_test ptx_ast)
install(TARGETS expr_test DESTINATION bin)

# Test executable for packed float types
add_executable(packed_float_test test/test_ptx_packed_float.c)
target_link_libraries(packed_float_test ptx_ast)
install(TARGETS packed_float_test DESTINATION bin)

# Test executable for the combined type system
add_executable(type_test test/test_ptx_type.c)
target_link_libraries(type_test ptx_ast)
install(TARGETS type_test DESTINATION bin)

# Test executable for the prelude utilities
add_executable(prelude_test tests/test_prelude.c)
install(TARGETS prelude_test DESTINATION bin) 