#pragma once

#include <ptx_ast/ptx_statement.h>
#include <stdbool.h>

/**
 * @brief Parse a PTX statement string and construct a ptx_statement_t
 * 
 * This function parses a complete PTX statement, which may include an optional label
 * followed by either a directive or an instruction.
 *
 * @param str The string to parse as a PTX statement
 * @param statement Pointer to ptx_statement_t* where the result will be stored
 * @return true if the string was successfully parsed as a statement, false otherwise
 */
bool parse_statement(const char* str, struct ptx_statement_t** statement); 