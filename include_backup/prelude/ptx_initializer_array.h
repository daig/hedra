/**
 * @file ptx_initializer_array.h
 * @brief Dynamic multi-dimensional array implementation using ptx_array_shape_t
 */

#pragma once

#include <stddef.h>
#include "prelude/ptx_array_shape.h"

/**
 * @typedef ptx_initializer_array_t
 * @brief Dynamic multi-dimensional array type
 * 
 * A heap-allocated array with dimensions specified by a ptx_array_shape_t.
 * The array stores elements of arbitrary size.
 */
typedef struct ptx_initializer_array {
    ptx_array_shape_t shape;  /**< Shape of the array */
    size_t element_size;      /**< Size of each element in bytes */
    void* data;               /**< Pointer to the array data */
} ptx_initializer_array_t;

/**
 * @brief Create a new initializer array
 * 
 * @param shape The shape of the array (will be copied)
 * @param element_size Size of each element in bytes
 * @return ptx_initializer_array_t* Newly allocated array (caller must free with ptx_initializer_array_free)
 */
ptx_initializer_array_t* ptx_initializer_array_create(const ptx_array_shape_t shape, size_t element_size);

/**
 * @brief Free an initializer array and all associated resources
 * 
 * @param array The array to free
 */
void ptx_initializer_array_free(ptx_initializer_array_t* array);

/**
 * @brief Get a pointer to an element in the array
 * 
 * @param array The array
 * @param indices Array of indices (one per dimension)
 * @return void* Pointer to the element, or NULL if indices are out of bounds
 */
void* ptx_initializer_array_get(const ptx_initializer_array_t* array, const size_t* indices);

/**
 * @brief Set an element in the array
 * 
 * @param array The array
 * @param indices Array of indices (one per dimension)
 * @param value Pointer to the value to set
 * @return int 0 on success, -1 if indices are out of bounds
 */
int ptx_initializer_array_set(ptx_initializer_array_t* array, const size_t* indices, const void* value);

/**
 * @brief Get the total number of elements in the array
 * 
 * @param array The array
 * @return size_t Total number of elements
 */
size_t ptx_initializer_array_elements(const ptx_initializer_array_t* array);

/**
 * @brief Get the shape of the array
 * 
 * @param array The array
 * @return const ptx_array_shape_t The array shape (do not free this pointer)
 */
const ptx_array_shape_t ptx_initializer_array_shape(const ptx_initializer_array_t* array);

/**
 * @brief Resize the array to a new shape
 * 
 * @param array The array to resize
 * @param new_shape The new shape (will be copied)
 * @return int 0 on success, -1 on failure
 * 
 * Note: This will reallocate the data array and may invalidate existing pointers.
 * The contents of the array are undefined after resizing.
 */
int ptx_initializer_array_resize(ptx_initializer_array_t* array, const ptx_array_shape_t new_shape); 