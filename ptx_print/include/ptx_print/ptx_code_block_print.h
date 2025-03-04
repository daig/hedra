/**
 * @file ptx_code_block_print.h
 * @brief Printer for PTX code blocks
 */

#ifndef PTX_CODE_BLOCK_PRINT_H
#define PTX_CODE_BLOCK_PRINT_H

#include <stdio.h>
#include <stdbool.h>
#include "ptx_ast/ptx_code_block.h"

/**
 * Print a PTX code block to a file
 * 
 * @param out The output file
 * @param block The code block to print
 * @param indent The indentation level (number of spaces)
 * @return true if successful, false otherwise
 */
bool ptx_print_code_block(FILE* out, const ptx_code_block_t* block, int indent);

/**
 * Print a PTX code block to a buffer
 * 
 * @param buffer The buffer to print to
 * @param buffer_size The size of the buffer
 * @param block The code block to print
 * @param indent The indentation level (number of spaces)
 * @return The number of characters printed, or negative on error
 */
int ptx_print_code_block_to_buffer(char* buffer, size_t buffer_size, const ptx_code_block_t* block, int indent);

#endif /* PTX_CODE_BLOCK_PRINT_H */ 