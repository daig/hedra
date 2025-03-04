#pragma once

#include <ptx_ast/ptx_constant.h>
#include <stdbool.h>

// Parse integer literal string and store in ptx_constant_t
// Returns true if successful, false if invalid format
bool parse_int_literal(const char* str, ptx_constant_t* constant);

// Parse floating-point literal string and store in ptx_constant_t
// Returns true if successful, false if invalid format
bool parse_float_literal(const char* str, ptx_constant_t* constant);

// Parse predicate literal string and store in ptx_constant_t
// In PTX, integer constants may be used as predicates, with 0 as False and non-zero as True
// Returns true if successful, false if invalid format
bool parse_pred_literal(const char* str, ptx_constant_t* constant); 

/**
 * @brief Parse a string as a PTX constant (integer, float, or predicate)
 * 
 * This function attempts to parse the input string as one of the PTX constant types:
 * - Integer (decimal, hexadecimal, octal, or binary)
 * - Floating-point (decimal or hexadecimal IEEE 754)
 * - Predicate (true or false)
 *
 * @param str The string to parse as a constant
 * @param constant Pointer to ptx_constant_t where the result will be stored
 * @return true if the string was successfully parsed as a constant, false otherwise
 */
bool parse_constant(const char* str, ptx_constant_t** constant); 

// Free a constant that was allocated by parse_constant
// This properly frees any dynamically allocated memory within the constant
void free_constant(ptx_constant_t* constant); 