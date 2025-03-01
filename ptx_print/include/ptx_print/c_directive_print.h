#pragma once

#include <ptx_ast/c_directive.h>
#include <stdio.h>

/**
 * Print a C-style directive (e.g., #include, #define) to the given file.
 * 
 * @param file The file to print to.
 * @param directive The type of C directive.
 * @param content The content of the directive.
 * @return The number of characters printed, or a negative value if an error occurs.
 */
int print_c_directive(FILE* file, enum c_directive_t directive, const char* content);

/**
 * Print a C-style directive (e.g., #include, #define) to a string.
 * 
 * @param str The string to print to.
 * @param size The size of the string.
 * @param directive The type of C directive.
 * @param content The content of the directive.
 * @return The number of characters printed, or a negative value if an error occurs.
 */
int sprint_c_directive(char* str, size_t size, enum c_directive_t directive, const char* content); 