#pragma once

#include <ptx_ast/>ptx_constant.h>
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