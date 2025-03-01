#pragma once

#include <ptx_ast/ptx_directive.h>
#include <stdbool.h>

/**
 * @brief Parse a PTX directive string and store in ptx_directive_t
 * 
 * This function takes a string representing a PTX directive (e.g., "version", "target", "func"),
 * and determines the corresponding directive type from the ptx_directive_t enum.
 *
 * @param str The directive string to parse (without the leading period)
 * @param directive Pointer to ptx_directive_t where the result will be stored
 * @return true if successful, false if the directive is not recognized
 */
bool parse_directive(const char* str, ptx_directive_t* directive); 