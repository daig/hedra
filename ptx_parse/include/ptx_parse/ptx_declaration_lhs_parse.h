/**
 * @file ptx_declaration_lhs_parse.h
 * @brief Functions for parsing PTX declaration left-hand side
 */

#ifndef HEDRA_PTX_DECLARATION_LHS_PARSE_H
#define HEDRA_PTX_DECLARATION_LHS_PARSE_H

#include <stdbool.h>
#include <ptx_ast/ptx_declaration_lhs.h>

/**
 * @brief Parse a PTX declaration LHS from a string
 * 
 * This function parses a PTX declaration left-hand side string (e.g., ".global .u32 loc",
 * ".reg .v4 .f32 accel", ".local .u16 kernel[19][19]") and populates the provided
 * ptx_declaration_type_t structure.
 * 
 * @param str String to parse
 * @param lhs Pointer to the declaration LHS structure to populate
 * @return bool True if parsing was successful, false otherwise
 */
bool parse_declaration_lhs(const char* str, ptx_declaration_type_t* lhs);

#endif /* HEDRA_PTX_DECLARATION_LHS_PARSE_H */ 