#pragma once

#include <ptx_ast/ptx_label.h>
#include <stdbool.h>

/**
 * @brief Parse a PTX label string and construct a ptx_label_t
 * 
 * This function takes a string representing a potential PTX label (e.g., "my_label:")
 * and creates a corresponding ptx_label_t struct if it's valid. A valid label
 * is an identifier followed by a colon.
 *
 * @param str The string to parse as a potential label
 * @param label Pointer to ptx_label_t* where the result will be stored
 * @return true if the string is a valid label and was successfully parsed, false otherwise
 */
bool parse_label(const char* str, ptx_label_t** label); 