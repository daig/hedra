/**
 * @file ptx_code_block_parse.h
 * @brief Parser for PTX code blocks
 */

#ifndef PTX_CODE_BLOCK_PARSE_H
#define PTX_CODE_BLOCK_PARSE_H

#include <stdbool.h>
#include "ptx_ast/ptx_code_block.h"

/**
 * @brief Parse a PTX code block enclosed in curly braces
 * 
 * Parses a complete code block starting with an opening brace '{' and
 * ending with a closing brace '}', including all statements inside the block.
 *
 * @param input The input string to parse
 * @param consumed The number of characters consumed by parsing
 * @param result The resulting code block AST node
 * @return true if parsing succeeded, false otherwise
 */
bool ptx_parse_code_block(const char* input, size_t* consumed, ptx_code_block_t** result);

#endif /* PTX_CODE_BLOCK_PARSE_H */ 