/**
 * @file ptx_declaration_lhs_print.h
 * @brief Functions for printing PTX declaration left-hand side
 */

#ifndef HEDRA_PTX_DECLARATION_LHS_PRINT_H
#define HEDRA_PTX_DECLARATION_LHS_PRINT_H

#include <stdio.h>
#include <stdbool.h>
#include <ptx_ast/ptx_declaration_lhs.h>

/**
 * @brief Print a PTX declaration LHS to a file
 * 
 * @param file File to print to
 * @param lhs The declaration LHS to print
 * @return bool True if successful, false otherwise
 */
bool print_declaration_lhs_to_file(FILE* file, const ptx_declaration_type_t* lhs);

/**
 * @brief Print a PTX declaration LHS to a buffer
 * 
 * @param buffer Buffer to print to
 * @param buffer_size Size of the buffer
 * @param lhs The declaration LHS to print
 * @return int Number of characters written, or negative on error
 */
int print_declaration_lhs_to_buffer(char* buffer, size_t buffer_size, const ptx_declaration_type_t* lhs);

#endif /* HEDRA_PTX_DECLARATION_LHS_PRINT_H */ 