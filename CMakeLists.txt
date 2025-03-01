cmake_minimum_required(VERSION 3.10)

# Project name and version
project(PTX_AST VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Prelude library source files
set(PRELUDE_SOURCES
    src/prelude/ptx_array_shape.c
    src/prelude/ptx_initializer_array.c
    src/prelude/ptx_initializer_helpers.c
    src/prelude/ptx_initializer_value_helpers.c
)

# Prelude library headers
set(PRELUDE_HEADERS
    include/prelude/prelude.h
    include/prelude/ptx_array_shape.h
    include/prelude/ptx_initializer_array.h
)

# AST library source files
set(AST_SOURCES
    src/ast/ptx_predefined_identifier.c
    src/ast/ptx_identifier.c
    src/ast/ptx_constant.c
    src/ast/ptx_constant_expr.c
    src/ast/ptx_type_fundamental.c
    src/ast/ptx_alternate_float_format.c
    src/ast/ptx_type_packed_float.c
    src/ast/ptx_type.c
    src/ast/ptx_scalar_float_type.c
    src/ast/ptx_statespace.c
    src/ast/ptx_type_graphics.c
)

# AST library headers
set(AST_HEADERS
    include/ast/ptx_predefined_identifier.h
    include/ast/ptx_identifier.h
    include/ast/ptx_constant.h
    include/ast/ptx_constant_expr.h
    include/ast/ptx_constent_expr.h
    include/ast/ptx_instruction.h
    include/ast/ptx_type_fundamental.h
    include/ast/ptx_alternate_float_format.h
    include/ast/ptx_type_packed_float.h
    include/ast/ptx_type.h
    include/ast/ptx_scalar_float_type.h
    include/ast/ptx_statespace.h
    include/ast/ptx_type_graphics.h
    include/ast/c_directive.h
    include/ast/comment.h
    include/ast/ptx_directive.h
    include/ast/ptx_label.h
    include/ast/ptx_statement.h
    include/ast/ptx_initializer_helpers.h
    include/ast/ptx_initializer_value_helpers.h
    include/ast/ptx_initializer_examples.h
    include/ast/ptx_type_decl.h
)

# Parse library source files
set(PARSE_SOURCES
    src/parse/ptx_constant_parse.c
)

# Parse library headers
set(PARSE_HEADERS
    include/parse/ptx_constant_parse.h
)

# Create prelude library
add_library(prelude STATIC ${PRELUDE_SOURCES})

# Create AST library
add_library(ptx_ast STATIC ${AST_SOURCES})

# Create parse library
add_library(ptx_parse STATIC ${PARSE_SOURCES})

# Setup dependencies between libraries
target_link_libraries(ptx_ast PRIVATE prelude)
target_link_libraries(ptx_parse PRIVATE ptx_ast prelude)

# Install targets
install(TARGETS prelude ptx_ast ptx_parse DESTINATION lib)
install(FILES ${PRELUDE_HEADERS} ${AST_HEADERS} ${PARSE_HEADERS} DESTINATION include)

# Test executables
# Commenting out tests with missing source files
# add_executable(ast_test tests/test_ast.c)
# target_link_libraries(ast_test ptx_ast)
# install(TARGETS ast_test DESTINATION bin)

add_executable(expr_test tests/test_constant_expr.c)
target_link_libraries(expr_test ptx_ast)
install(TARGETS expr_test DESTINATION bin)

# add_executable(packed_float_test tests/test_ptx_packed_float.c)
# target_link_libraries(packed_float_test ptx_ast)
# install(TARGETS packed_float_test DESTINATION bin)

# add_executable(type_test tests/test_ptx_type.c)
# target_link_libraries(type_test ptx_ast)
# install(TARGETS type_test DESTINATION bin)

add_executable(prelude_test tests/test_prelude.c)
target_link_libraries(prelude_test prelude)
install(TARGETS prelude_test DESTINATION bin)

add_executable(initializer_array_test tests/test_initializer_array.c)
target_link_libraries(initializer_array_test prelude)
install(TARGETS initializer_array_test DESTINATION bin)

# Test for initializer helpers
add_executable(initializer_test tests/test_initializer.c)
target_link_libraries(initializer_test prelude)
install(TARGETS initializer_test DESTINATION bin)

# Test for constant parsing
add_executable(constant_parse_test tests/test_constant_parse.c)
target_link_libraries(constant_parse_test ptx_parse)
install(TARGETS constant_parse_test DESTINATION bin) 