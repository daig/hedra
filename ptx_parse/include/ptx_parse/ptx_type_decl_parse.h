/**
 * @file ptx_type_decl_parse.h
 * @brief Functions for parsing PTX declarations
 */

#ifndef HEDRA_PTX_TYPE_DECL_PARSE_H
#define HEDRA_PTX_TYPE_DECL_PARSE_H

#include <stdbool.h>
#include <ptx_ast/ptx_type_decl.h>

/**
 * @brief Parse a PTX declaration from a string
 * 
 * This function parses a complete PTX declaration including the declaration LHS and
 * any initializer (if present). It handles both scalar initializers and array initializers
 * based on the shape of the declaration.
 * 
 * @param str String to parse
 * @param decl Pointer to the declaration structure to populate
 * @return bool True if parsing was successful, false otherwise
 */
bool parse_ptx_decl(const char* str, ptx_decl_t* decl);

/**
 * @brief Free resources allocated for a PTX declaration
 * 
 * This function frees all dynamically allocated memory associated with a PTX declaration,
 * including the declaration LHS and any initializers.
 * 
 * @param decl Pointer to the declaration structure to free
 */
void free_ptx_decl(ptx_decl_t* decl);

#endif /* HEDRA_PTX_TYPE_DECL_PARSE_H */ 