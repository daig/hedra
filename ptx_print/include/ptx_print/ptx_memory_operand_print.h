#pragma once
#include <ptx_ast/ptx_memory_operand.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * Print a memory operand to a file stream
 * 
 * @param file The file stream to print to
 * @param operand The memory operand to print
 * @return true if printing was successful, false otherwise
 */
bool print_memory_operand_to_file(FILE* file, const ptx_memory_operand_t* operand);

/**
 * Print a memory operand to a string buffer
 * 
 * @param buffer The buffer to print to
 * @param buffer_size The size of the buffer
 * @param operand The memory operand to print
 * @return The number of characters printed, or -1 if an error occurred
 */
int print_memory_operand_to_buffer(char* buffer, size_t buffer_size, const ptx_memory_operand_t* operand); 