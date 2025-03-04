/**
 * @file ptx_function_parse.h
 * @brief Parser for PTX function declarations
 */

#ifndef PTX_FUNCTION_PARSE_H
#define PTX_FUNCTION_PARSE_H

#include <stdbool.h>
#include "ptx_ast/ptx_function.h"

/**
 * @brief Parse a PTX function declaration
 * 
 * @param input The input string to parse
 * @param consumed The number of characters consumed by parsing
 * @param result The resulting function AST node
 * @return true if parsing succeeded, false otherwise
 */
bool ptx_parse_function_declaration(const char* input, size_t* consumed, ptx_function_t* result);

/**
 * @brief Parse a PTX function parameter list
 * 
 * @param input The input string to parse
 * @param consumed The number of characters consumed by parsing
 * @param result The resulting parameter list
 * @return true if parsing succeeded, false otherwise
 */
bool ptx_parse_parameter_list(const char* input, size_t* consumed, ptx_parameter_t** result);

#endif /* PTX_FUNCTION_PARSE_H */ 