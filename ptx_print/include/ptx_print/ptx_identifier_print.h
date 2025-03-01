#pragma once

#include <ptx_ast/ptx_identifier.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Prints a PTX identifier to a file stream
 * 
 * @param file The file stream to print to
 * @param identifier The identifier to print
 * @return bool true if successful, false otherwise
 */
bool print_identifier_to_file(FILE* file, ptx_identifier_t identifier);

/**
 * @brief Prints a PTX identifier to a string buffer
 * 
 * @param buffer The buffer to print to
 * @param buffer_size The size of the buffer
 * @param identifier The identifier to print
 * @return int The number of characters printed, or negative on error
 */
int print_identifier_to_buffer(char* buffer, size_t buffer_size, ptx_identifier_t identifier); 