cmake_minimum_required(VERSION 3.10)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# List of source files
set(SOURCES
    src/ptx_directive_print.c
    src/ptx_instruction_print.c
    src/ptx_label_print.c
    src/ptx_constant_print.c
    src/ptx_identifier_print.c
    src/ptx_type_print.c
    src/ptx_statement_print.c
)

# List of header files
set(HEADERS
    include/ptx_print/ptx_directive_print.h
    include/ptx_print/ptx_instruction_print.h
    include/ptx_print/ptx_label_print.h
    include/ptx_print/ptx_constant_print.h
    include/ptx_print/ptx_identifier_print.h
    include/ptx_print/ptx_type_print.h
    include/ptx_print/ptx_statement_print.h
)

# Create the library
add_library(ptx_print ${SOURCES} ${HEADERS})

# Set include directories
target_include_directories(ptx_print
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link against dependencies
target_link_libraries(ptx_print
    PUBLIC
        ptx_ast
) 