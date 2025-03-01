#pragma once
#include <ptx_ast/ptx_identifier.h>
#include <stdbool.h>

/**
 * @brief Parse a PTX identifier string and construct a ptx_identifier_t
 * 
 * @param str The string to parse as a potential identifier
 * @param identifier Pointer to ptx_identifier_t* where the result will be stored
 * @return true if the string is a valid identifier and was successfully parsed, false otherwise
 */
bool parse_identifier(const char* str, ptx_identifier_t** identifier); 