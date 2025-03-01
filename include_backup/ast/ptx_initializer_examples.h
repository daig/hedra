/**
 * @file ptx_initializer_examples.h
 * @brief Example functions demonstrating how to use the helper functions
 */

#pragma once

#include "ast/ptx_type_decl.h"

/**
 * @brief Create an example of a global 2D array of 32-bit signed integers with initializer
 * 
 * This represents the PTX declaration: .global .s32 offset[][2] = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };
 * 
 * @return ptx_type_decl_t* A dynamically allocated type declaration (caller must free)
 */
ptx_type_decl_t* create_example_global_s32_array_with_initializer(void);

/**
 * @brief Create an example of a global vector of mixed values
 * 
 * This represents the PTX declaration: .global .v4 .u32 mixed = {10, bar, add_func, 0};
 * 
 * @return ptx_type_decl_t* A dynamically allocated type declaration (caller must free)
 */
ptx_type_decl_t* create_example_global_v4_u32_mixed(void);

/**
 * @brief Create an example of masked address and masked integer initializers
 * 
 * This represents the PTX declarations:
 * .global .u8 masked_byte = mask(foo, 0xFF);  // Extract byte 0 of the address of foo
 * .global .u8 masked_int = mask(0x12345678, 0xFF00);  // Extract byte 1 of the integer
 * 
 * @return ptx_type_decl_t** Array of two dynamically allocated type declarations (caller must free each)
 */
ptx_type_decl_t** create_example_masked_initializers(void);

/**
 * @brief Free a dynamically allocated type declaration
 * 
 * @param decl The type declaration to free
 */
void free_example_type_decl(ptx_type_decl_t* decl); 