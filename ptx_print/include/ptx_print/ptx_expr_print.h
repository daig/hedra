#pragma once

#include <ptx_ast/ptx_constant_expr.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Print a constant expression to a file stream
 * 
 * @param file The file stream to print to
 * @param expr The expression to print
 * @return true if printing succeeded, false otherwise
 */
bool print_expr_to_file(FILE* file, ptx_expr_t* expr);

/**
 * @brief Print a constant expression to a string buffer
 * 
 * @param buffer The buffer to print to
 * @param buffer_size The size of the buffer
 * @param expr The expression to print
 * @return The number of characters printed, or -1 on error
 */
int print_expr_to_buffer(char* buffer, size_t buffer_size, ptx_expr_t* expr); 