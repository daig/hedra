#pragma once

#include <ptx_ast/ptx_type.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Prints a PTX type to a file stream
 * 
 * @param file The file stream to print to
 * @param type The PTX type to print
 * @return bool true if successful, false otherwise
 */
bool print_type_to_file(FILE* file, ptx_type_t type);

/**
 * @brief Prints a PTX type to a string buffer
 * 
 * @param buffer The buffer to print to
 * @param buffer_size The size of the buffer
 * @param type The PTX type to print
 * @return int The number of characters printed, or negative on error
 */
int print_type_to_buffer(char* buffer, size_t buffer_size, ptx_type_t type); 