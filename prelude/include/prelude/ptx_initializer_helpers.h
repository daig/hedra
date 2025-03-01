/**
 * @file ptx_initializer_helpers.h
 * @brief Helper functions for creating and managing PTX initializers
 * 
 * This header provides a comprehensive set of functions for creating and managing
 * initializers in PTX (Parallel Thread Execution) code. These helper functions simplify
 * the process of creating complex initializer structures by handling the allocation,
 * initialization, and memory management.
 * 
 * The initializer system is designed to work with the following components:
 * 1. ptx_initializer_value_t - Individual values (int, float, var address, etc.)
 * 2. ptx_initializer_array_t - Collections of initializer values for arrays/vectors 
 * 3. ptx_initializer_t - The complete initializer that can be attached to a variable
 * 
 * The functions in this file work closely with those in ptx_initializer_value_helpers.h,
 * which provide constructors for creating individual initializer values.
 * 
 * For example usage, see the test_initializer.c file in the tests directory.
 */

#pragma once

#include <ptx_ast/ptx_type_decl.h>
#include <prelude/ptx_initializer_array.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Create a new scalar initializer
 * 
 * @param kind The kind of scalar initializer (INIT_SCALAR_INT, INIT_SCALAR_FLOAT, etc.)
 * @param value Pointer to the initializer value
 * @return ptx_initializer_t The created initializer
 */
ptx_initializer_t ptx_initializer_create_scalar(ptx_initializer_kind_t kind, const ptx_initializer_value_t* value);

/**
 * @brief Create a new vector initializer
 * 
 * @param size Size of the vector (2 or 4)
 * @param values Array of initializer values
 * @return ptx_initializer_t The created initializer
 */
ptx_initializer_t ptx_initializer_create_vector(ptx_vector_size_t size, const ptx_initializer_value_t* values);

/**
 * @brief Create a new array initializer
 * 
 * @param shape The shape of the array
 * @return ptx_initializer_t The created initializer
 */
ptx_initializer_t ptx_initializer_create_array(const ptx_array_shape_t shape);

/**
 * @brief Set a value in a vector or array initializer
 * 
 * @param initializer Pointer to the initializer
 * @param indices Array of indices (one per dimension)
 * @param value Pointer to the initializer value to set
 * @return int 0 on success, -1 if indices are out of bounds or initializer is not a vector/array
 */
int ptx_initializer_set_value(ptx_initializer_t* initializer, const size_t* indices, const ptx_initializer_value_t* value);

/**
 * @brief Get a value from a vector or array initializer
 * 
 * @param initializer Pointer to the initializer
 * @param indices Array of indices (one per dimension)
 * @param value Pointer to store the retrieved initializer value
 * @return int 0 on success, -1 if indices are out of bounds or initializer is not a vector/array
 */
int ptx_initializer_get_value(const ptx_initializer_t* initializer, const size_t* indices, ptx_initializer_value_t* value);

/**
 * @brief Free an initializer
 * 
 * Frees all memory associated with an initializer, including any
 * arrays or strings it contains. This function will:
 * 
 * 1. Check if the initializer is NULL (and return early if so)
 * 2. For vector or array initializers, free the associated ptx_initializer_array_t
 * 3. For other types, free any associated dynamic memory
 * 
 * After calling this function, the initializer will be in a valid but
 * empty state with any pointer fields set to NULL.
 * 
 * @param initializer Pointer to the initializer to free
 */
void ptx_initializer_free(ptx_initializer_t* initializer);

/**
 * @brief Create an integer initializer
 * 
 * Creates a new initializer containing a single integer value.
 * This is the simplest form of initializer for integer constants in PTX.
 * 
 * Example PTX:
 * ```
 * .global .u32 my_int = 42;
 * ```
 * 
 * @param value The integer value
 * @return ptx_initializer_t The created initializer
 */
ptx_initializer_t ptx_initializer_create_int(int64_t value);

/**
 * @brief Create a floating-point initializer
 * 
 * Creates a new initializer containing a single floating-point value.
 * This is the simplest form of initializer for floating-point constants in PTX.
 * 
 * Example PTX:
 * ```
 * .global .f32 my_float = 3.14;
 * ```
 * 
 * @param value The floating-point value
 * @return ptx_initializer_t The created initializer
 */
ptx_initializer_t ptx_initializer_create_float(double value);

/**
 * @brief Create a variable address initializer
 * 
 * Creates a new initializer containing a variable address, with optional
 * offset and generic flag. This initializes a variable with the address of
 * another variable.
 * 
 * Example PTX:
 * ```
 * .global .u32 ptr = my_var;           // Address of my_var
 * .global .u32 ptr_gen = generic(my_var); // Generic address of my_var
 * .global .u32 ptr_off = my_var+4;     // Address of my_var + 4 bytes
 * ```
 * 
 * @param var_name Name of the variable
 * @param offset Optional offset (0 for no offset)
 * @param is_generic Whether to use the generic() operator
 * @return ptx_initializer_t The created initializer
 */
ptx_initializer_t ptx_initializer_create_var_addr(const char* var_name, int64_t offset, bool is_generic);

/**
 * @brief Create a function address initializer
 * 
 * Creates a new initializer containing a function address.
 * This initializes a variable with the address of a function.
 * 
 * Example PTX:
 * ```
 * .global .u32 func_ptr = my_kernel;  // Address of my_kernel function
 * ```
 * 
 * @param func_name Name of the function
 * @return ptx_initializer_t The created initializer
 */
ptx_initializer_t ptx_initializer_create_func_addr(const char* func_name);

/**
 * @brief Create a masked variable address initializer
 * 
 * Creates a new initializer containing a masked variable address. This is used to
 * extract a specific byte from a variable address.
 * 
 * The mask() operator in PTX extracts consecutive bits from a variable address.
 * This is primarily used for byte extraction from addresses.
 * 
 * Example PTX:
 * ```
 * .global .u8 byte0 = mask(my_var, 0xFF);  // Extract byte 0 of my_var's address
 * ```
 * 
 * @param var_name Name of the variable
 * @param offset Optional offset (0 for no offset)
 * @param is_generic Whether to use the generic() operator
 * @param mask The mask value (e.g., PTX_MASK_BYTE0 for the first byte)
 * @return ptx_initializer_t The created initializer
 */
ptx_initializer_t ptx_initializer_create_masked_var_addr(const char* var_name, int64_t offset, bool is_generic, ptx_mask_value_t mask);

/**
 * @brief Create a masked integer initializer
 * 
 * Creates a new initializer containing a masked integer. This is used to
 * extract a specific byte from an integer constant.
 * 
 * Example PTX:
 * ```
 * .global .u8 byte0 = mask(0x12345678, 0xFF);  // Extract byte 0 (0x78)
 * ```
 * 
 * @param value The integer value
 * @param mask The mask value (e.g., PTX_MASK_BYTE1 for the second byte)
 * @return ptx_initializer_t The created initializer
 */
ptx_initializer_t ptx_initializer_create_masked_int(int64_t value, ptx_mask_value_t mask);

/**
 * @brief Create an array initializer
 * 
 * Creates a new initializer containing an array of values. This is used for
 * multi-dimensional arrays or composite initializers in PTX.
 * 
 * Example PTX:
 * ```
 * .global .s32 my_array[4] = {1, 2, 3, 4};
 * ```
 * 
 * @param shape The shape of the array
 * @return ptx_initializer_t The created initializer
 */
ptx_initializer_t ptx_initializer_create_array(const ptx_array_shape_t shape); 