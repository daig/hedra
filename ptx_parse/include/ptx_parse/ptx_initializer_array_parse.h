#pragma once

#include <ptx_ast/ptx_type_decl.h>
#include <prelude/ptx_initializer_array.h>
#include <prelude/ptx_array_shape.h>
#include <stdbool.h>

/**
 * @brief Parse an initializer array from a string
 * 
 * @param str String to parse
 * @param pos Pointer to current position in string (will be updated)
 * @param array_shape Shape of the array to parse
 * @param array Pointer to initializer array pointer that will be allocated and filled
 * @return true if parsing successful, false otherwise
 */
bool parse_initializer_array(const char* str, int* pos, ptx_array_shape_t array_shape, ptx_initializer_array_t** array); 