#pragma once

#include <ptx_ast/ptx_type_decl.h>
#include <stdio.h>

/**
 * Print a PTX initializer value to the given file.
 * This handles scalar values, variable addresses, and masked values.
 * 
 * @param file The file to print to.
 * @param value The initializer value to print.
 * @return The number of characters printed, or a negative value if an error occurs.
 */
int print_ptx_initializer_value(FILE* file, const ptx_initializer_value_t* value);

/**
 * Print a PTX initializer value to a string.
 * This handles scalar values, variable addresses, and masked values.
 * 
 * @param str The string to print to.
 * @param size The size of the string.
 * @param value The initializer value to print.
 * @return The number of characters printed, or a negative value if an error occurs.
 */
int sprint_ptx_initializer_value(char* str, size_t size, const ptx_initializer_value_t* value); 