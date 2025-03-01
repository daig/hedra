#pragma once

#include <ptx_ast/ptx_statement.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Prints a PTX statement to a file stream
 * 
 * @param file The file stream to print to
 * @param statement The statement to print
 * @return bool true if successful, false otherwise
 */
bool print_statement_to_file(FILE* file, const ptx_statement_t* statement);

/**
 * @brief Prints a PTX statement to a string buffer
 * 
 * @param buffer The buffer to print to
 * @param buffer_size The size of the buffer
 * @param statement The statement to print
 * @return int The number of characters printed, or negative on error
 */
int print_statement_to_buffer(char* buffer, size_t buffer_size, const ptx_statement_t* statement); 