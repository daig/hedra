#pragma once

#include <ptx_ast/ptx_directive.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Prints a PTX directive to a file stream
 * 
 * @param file The file stream to print to
 * @param directive The directive to print
 * @return bool true if successful, false otherwise
 */
bool print_directive_to_file(FILE* file, ptx_directive_t directive);

/**
 * @brief Prints a PTX directive to a string buffer
 * 
 * @param buffer The buffer to print to
 * @param buffer_size The size of the buffer
 * @param directive The directive to print
 * @return int The number of characters printed, or negative on error
 */
int print_directive_to_buffer(char* buffer, size_t buffer_size, ptx_directive_t directive);

/**
 * @brief Gets the string representation of a PTX directive
 * 
 * @param directive The directive to get the string for
 * @return const char* The string representation, or NULL if not found
 */
const char* get_directive_string(ptx_directive_t directive); 