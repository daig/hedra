#pragma once

#include <ptx_ast/ptx_constant.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Prints a PTX constant to a file stream
 * 
 * @param file The file stream to print to
 * @param constant The constant to print
 * @return bool true if successful, false otherwise
 */
bool print_constant_to_file(FILE* file, ptx_constant_t constant);

/**
 * @brief Prints a PTX constant to a string buffer
 * 
 * @param buffer The buffer to print to
 * @param buffer_size The size of the buffer
 * @param constant The constant to print
 * @return int The number of characters printed, or negative on error
 */
int print_constant_to_buffer(char* buffer, size_t buffer_size, ptx_constant_t constant); 