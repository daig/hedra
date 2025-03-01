cmake_minimum_required(VERSION 3.10)

# Library name and version
project(ptx_parse VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/ptx_constant_parse.c
    src/ptx_constant_expr_parse.c
    src/ptx_directive_parse.c
    src/ptx_identifier_parse.c
    src/ptx_instruction_parse.c
    src/ptx_label_parse.c
    src/ptx_statement_parse.c
    src/ptx_type_parse.c
)

# Header files
set(HEADERS
    include/ptx_parse.h
    include/ptx_parse/ptx_constant_parse.h
    include/ptx_parse/ptx_constant_expr_parse.h
    include/ptx_parse/ptx_directive_parse.h
    include/ptx_parse/ptx_identifier_parse.h
    include/ptx_parse/ptx_instruction_parse.h
    include/ptx_parse/ptx_label_parse.h
    include/ptx_parse/ptx_statement_parse.h
    include/ptx_parse/ptx_type_parse.h
)

# Create library
add_library(ptx_parse STATIC ${SOURCES})

# Set include directories for targets that link against this library
target_include_directories(ptx_parse
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Set dependencies - use direct target references instead of find_package
target_link_libraries(ptx_parse PRIVATE prelude ptx_ast)

# Install targets
install(TARGETS ptx_parse 
    EXPORT ptx_parseTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# Install headers
install(FILES ${HEADERS} DESTINATION include)
install(DIRECTORY include/ptx_parse DESTINATION include)

# Export targets
install(EXPORT ptx_parseTargets
    FILE ptx_parseTargets.cmake
    NAMESPACE ptx_parse::
    DESTINATION lib/cmake/ptx_parse
) 