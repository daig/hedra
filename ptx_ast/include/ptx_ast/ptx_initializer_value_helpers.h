/**
 * @file ptx_initializer_value_helpers.h
 * @brief Helper constructor functions for ptx_initializer_value_t variants
 * 
 * This header provides a set of functions for creating various types of initializer values
 * used in PTX declarations. These helper functions simplify the process of creating complex
 * initializer value structures by handling all the necessary initialization and setup.
 * 
 * Each function creates a specific type of initializer value with the appropriate kind
 * and initialized fields. These values can then be used with the functions in
 * ptx_initializer_helpers.h to create complete initializers for PTX variables.
 * 
 * Example usage:
 * ```c
 * // Create an integer initializer value
 * ptx_initializer_value_t int_val = ptx_initializer_value_int(42);
 * 
 * // Create a function address value
 * ptx_initializer_value_t func_val = ptx_initializer_value_func_addr("my_kernel");
 * 
 * // Create a masked integer value (for byte extraction)
 * ptx_initializer_value_t masked = ptx_initializer_value_masked_int(0x12345678, PTX_MASK_BYTE1);
 * ```
 */

#pragma once

#include <ptx_ast/>ptx_type_decl.h>
#include <stdbool.h>

/**
 * @brief Create an integer initializer value
 * 
 * Creates a ptx_initializer_value_t of kind INIT_SCALAR_INT with the provided integer value.
 * This can be used for initializing integer constants in PTX.
 * 
 * @param value The integer value
 * @return ptx_initializer_value_t The created value
 */
ptx_initializer_value_t ptx_initializer_value_int(int64_t value);

/**
 * @brief Create a floating-point initializer value
 * 
 * Creates a ptx_initializer_value_t of kind INIT_SCALAR_FLOAT with the provided floating-point value.
 * This can be used for initializing floating-point constants in PTX.
 * 
 * @param value The floating-point value
 * @return ptx_initializer_value_t The created value
 */
ptx_initializer_value_t ptx_initializer_value_float(double value);

/**
 * @brief Create a variable address initializer value
 * 
 * Creates a ptx_initializer_value_t of kind INIT_VAR_ADDR with the provided variable name,
 * optional offset, and generic flag. This can be used for initializing a variable with the
 * address of another variable, with an optional byte offset added to the address.
 * 
 * In PTX, the generic() operator can be used to get the generic address of a variable.
 * If is_generic is true, the generic() operator will be applied to the variable address.
 *
 * Example PTX:
 * ```
 * .global .u32 p1 = global_var;           // offset of global_var in .global space
 * .global .u32 p2 = generic(global_var);  // generic address of global_var
 * .global .u32 p3 = global_var+8;         // offset of global_var+8 bytes
 * ```
 * 
 * @param var_name Name of the variable
 * @param offset Optional offset (0 for no offset)
 * @param is_generic Whether to use the generic() operator
 * @return ptx_initializer_value_t The created value
 */
ptx_initializer_value_t ptx_initializer_value_var_addr(const char* var_name, int64_t offset, bool is_generic);

/**
 * @brief Create a function address initializer value
 * 
 * Creates a ptx_initializer_value_t of kind INIT_FUNC_ADDR with the provided function name.
 * This can be used for initializing a variable with the address of a function.
 * 
 * Example PTX:
 * ```
 * .global .u32 func_ptr = my_kernel;  // Address of my_kernel function
 * ```
 * 
 * @param func_name Name of the function
 * @return ptx_initializer_value_t The created value
 */
ptx_initializer_value_t ptx_initializer_value_func_addr(const char* func_name);

/**
 * @brief Create a masked variable address initializer value
 * 
 * Creates a ptx_initializer_value_t of kind INIT_MASKED_VAR_ADDR with the provided variable name,
 * optional offset, generic flag, and mask value. This can be used for initializing a variable with
 * a specific byte extracted from the address of another variable.
 * 
 * The mask() operator in PTX extracts consecutive bits from an expression and inserts these bits at 
 * the lowest position of the initialized variable. This is primarily used for byte extraction
 * from addresses. The mask values (PTX_MASK_BYTE0, PTX_MASK_BYTE1, etc.) specify which byte to extract.
 * 
 * Example PTX:
 * ```
 * .global .u8 byte0 = mask(global_var, 0xFF);          // Extract byte 0 of global_var's address
 * .global .u8 byte1 = mask(generic(global_var), 0xFF00); // Extract byte 1 of global_var's generic address
 * ```
 * 
 * @param var_name Name of the variable
 * @param offset Optional offset (0 for no offset)
 * @param is_generic Whether to use the generic() operator
 * @param mask The mask value (e.g., PTX_MASK_BYTE0 to extract the first byte)
 * @return ptx_initializer_value_t The created value
 */
ptx_initializer_value_t ptx_initializer_value_masked_var_addr(const char* var_name, int64_t offset, bool is_generic, ptx_mask_value_t mask);

/**
 * @brief Create a masked integer initializer value
 * 
 * Creates a ptx_initializer_value_t of kind INIT_MASKED_INT with the provided integer value
 * and mask value. This can be used for initializing a variable with a specific byte extracted
 * from an integer constant.
 * 
 * The mask() operator in PTX extracts consecutive bits from an expression. For integer constants,
 * this is typically used to extract specific bytes from a larger integer.
 * 
 * Example PTX:
 * ```
 * .global .u8 byte0 = mask(0x12345678, 0xFF);          // Extract byte 0 (0x78)
 * .global .u8 byte1 = mask(0x12345678, 0xFF00);        // Extract byte 1 (0x56)
 * ```
 * 
 * @param value The integer value
 * @param mask The mask value (e.g., PTX_MASK_BYTE1 to extract the second byte)
 * @return ptx_initializer_value_t The created value
 */
ptx_initializer_value_t ptx_initializer_value_masked_int(int64_t value, ptx_mask_value_t mask); 