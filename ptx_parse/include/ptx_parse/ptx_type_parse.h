#pragma once

#include <ptx_ast/ptx_type.h>
#include <stdbool.h>

/**
 * @brief Parse a string as a PTX type (.s32, .f64, .v4.u8, etc.)
 * 
 * This function attempts to parse the input string as a PTX type,
 * which can be a fundamental type (like .s32, .f64), a vector type
 * (like .v2.f32), a graphics type (like .texref), or an alternate
 * float format (like .bf16).
 *
 * @param str The string to parse as a PTX type (should start with a period)
 * @param type Pointer to ptx_type_t where the result will be stored
 * @return true if the string was successfully parsed as a type, false otherwise
 */
bool parse_type(const char* str, ptx_type_t* type); 