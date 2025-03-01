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
    include/prelude.h
    include/ptx_array_shape.h
    include/ptx_initializer_array.h
    include/ptx_initializer_helpers.h
    include/ptx_initializer_value_helpers.h
    include/ptx_type_decl.h
)

# AST library source files
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

# AST library headers
set(AST_HEADERS
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
    include/ptx_statespace.h
)

# Create prelude library
add_library(prelude STATIC ${PRELUDE_SOURCES})

# Create AST library
add_library(ptx_ast STATIC ${AST_SOURCES})

# Install targets
install(TARGETS prelude ptx_ast DESTINATION lib)
install(FILES ${PRELUDE_HEADERS} ${AST_HEADERS} DESTINATION include)

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