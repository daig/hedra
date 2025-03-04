#ifndef PTX_FUNCTION_PRINT_H
#define PTX_FUNCTION_PRINT_H

#include <stdio.h>
#include <stdbool.h>
#include "ptx_ast/ptx_function.h"

/**
 * Print a PTX function parameter
 * 
 * @param out The output file
 * @param param The parameter to print
 * @return true if successful, false otherwise
 */
bool ptx_print_parameter(FILE* out, const ptx_parameter_t* param);

/**
 * Print a PTX function declaration
 * 
 * @param out The output file
 * @param function The function to print
 * @return true if successful, false otherwise
 */
bool ptx_print_function(FILE* out, const ptx_function_t* function);

#endif /* PTX_FUNCTION_PRINT_H */ 