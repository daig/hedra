#pragma once

#include <ptx_ast/ptx_statespace.h>
#include <stdbool.h>

/**
 * @brief Parse a string as a PTX state space (.reg, .global, .shared, etc.)
 * 
 * This function attempts to parse the input string as a PTX state space,
 * which represents different memory spaces in the PTX architecture.
 *
 * @param str The string to parse as a PTX state space (should start with a period)
 * @param space Pointer to ptx_state_space_t where the result will be stored
 * @return true if the string was successfully parsed as a state space, false otherwise
 */
bool parse_state_space(const char* str, ptx_state_space_t* space); 