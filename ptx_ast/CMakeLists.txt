cmake_minimum_required(VERSION 3.10)

# Library name and version
project(ptx_ast VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/ptx_predefined_identifier.c
    src/ptx_identifier.c
    src/ptx_constant.c
    src/ptx_constant_expr.c
    src/ptx_type_fundamental.c
    src/ptx_alternate_float_format.c
    src/ptx_type_packed_float.c
    src/ptx_type.c
    src/ptx_scalar_float_type.c
    src/ptx_statespace.c
    src/ptx_type_graphics.c
)

# Header files
set(HEADERS
    include/ptx_ast.h
    include/ptx_ast/c_directive.h
    include/ptx_ast/comment.h
    include/ptx_ast/ptx_alternate_float_format.h
    include/ptx_ast/ptx_constant.h
    include/ptx_ast/ptx_constant_expr.h
    include/ptx_ast/ptx_constent_expr.h
    include/ptx_ast/ptx_directive.h
    include/ptx_ast/ptx_identifier.h
    include/ptx_ast/ptx_initializer_examples.h
    include/ptx_ast/ptx_initializer_helpers.h
    include/ptx_ast/ptx_initializer_value_helpers.h
    include/ptx_ast/ptx_instruction.h
    include/ptx_ast/ptx_label.h
    include/ptx_ast/ptx_predefined_identifier.h
    include/ptx_ast/ptx_scalar_float_type.h
    include/ptx_ast/ptx_statement.h
    include/ptx_ast/ptx_statespace.h
    include/ptx_ast/ptx_type.h
    include/ptx_ast/ptx_type_decl.h
    include/ptx_ast/ptx_type_fundamental.h
    include/ptx_ast/ptx_type_graphics.h
    include/ptx_ast/ptx_type_packed_float.h
)

# Create library
add_library(ptx_ast STATIC ${SOURCES})

# Set include directories for targets that link against this library
target_include_directories(ptx_ast
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Set dependencies
find_package(prelude REQUIRED)
target_link_libraries(ptx_ast PRIVATE prelude::prelude)

# Install targets
install(TARGETS ptx_ast 
    EXPORT ptx_astTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# Install headers
install(FILES ${HEADERS} DESTINATION include)
install(DIRECTORY include/ptx_ast DESTINATION include)

# Export targets
install(EXPORT ptx_astTargets
    FILE ptx_astTargets.cmake
    NAMESPACE ptx_ast::
    DESTINATION lib/cmake/ptx_ast
) 