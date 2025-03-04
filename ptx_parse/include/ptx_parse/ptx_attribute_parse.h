/**
 * @file ptx_attribute_parse.h
 * @brief Functions for parsing PTX variable and function attributes
 */

#ifndef HEDRA_PTX_ATTRIBUTE_PARSE_H
#define HEDRA_PTX_ATTRIBUTE_PARSE_H

#include <stdbool.h>
#include <ptx_ast/ptx_declaration_lhs.h>

/**
 * @brief Parse a PTX attribute tag from a string
 * 
 * This function parses a PTX attribute tag string (e.g., ".managed", ".unified")
 * and populates the provided ptx_attribute_tag_t.
 * 
 * @param str String to parse
 * @param tag Pointer to the attribute tag enum to populate
 * @param str_end Pointer to store the position after the parsed tag
 * @return bool True if parsing was successful, false otherwise
 */
bool parse_attribute_tag(const char* str, ptx_attribute_tag_t* tag, const char** str_end);

/**
 * @brief Parse a complete PTX attribute from a string
 * 
 * This function parses a complete PTX attribute directive including the tag and
 * any parameters (like UUID for unified attributes).
 * 
 * @param str String to parse
 * @param attr Pointer to the attribute structure to populate
 * @param str_end Pointer to store the position after the parsed attribute
 * @return bool True if parsing was successful, false otherwise
 */
bool parse_attribute(const char* str, ptx_attribute_t* attr, const char** str_end);

#endif /* HEDRA_PTX_ATTRIBUTE_PARSE_H */ 