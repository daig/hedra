/**
 * @file ptx_array_shape.h
 * @brief Array shape type and functions for multi-dimensional arrays
 */

#ifndef HEDRA_PTX_ARRAY_SHAPE_H
#define HEDRA_PTX_ARRAY_SHAPE_H

#include <stddef.h>
#include <stdbool.h>

/**
 * @typedef ptx_array_shape_t
 * @brief Variable length array shape type
 * 
 * A null-terminated sequence of size_t values representing array dimensions.
 * The first trailing 0 indicates that dimension and all higher dimensions have 0 extent.
 * 
 * Examples:
 *   - {3, 4, 0} represents a 3x4 2D array
 *   - {5, 0} represents a 1D array with 5 elements
 *   - {0} represents an empty array
 */
typedef size_t* ptx_array_shape_t;

/**
 * @brief Create a new array shape
 * 
 * @param dims Number of dimensions
 * @param ... Dimension sizes (as size_t values)
 * @return ptx_array_shape_t Newly allocated array shape (caller must free)
 */
ptx_array_shape_t ptx_array_shape_create(size_t dims, ...);

/**
 * @brief Copy an array shape
 * 
 * @param shape Source shape to copy
 * @return ptx_array_shape_t New copy of the shape (caller must free)
 */
ptx_array_shape_t ptx_array_shape_copy(const ptx_array_shape_t shape);

/**
 * @brief Get the number of dimensions in an array shape
 * 
 * @param shape The array shape
 * @return size_t Number of dimensions
 */
size_t ptx_array_shape_ndims(const ptx_array_shape_t shape);

/**
 * @brief Free an array shape
 * 
 * @param shape The array shape to free
 */
void ptx_array_shape_free(ptx_array_shape_t shape);

/**
 * @brief Calculate the total number of elements in an array with the given shape
 * 
 * @param shape The array shape
 * @return size_t Total number of elements
 */
size_t ptx_array_shape_elements(const ptx_array_shape_t shape);

/**
 * @brief Compare two array shapes for equality
 * 
 * @param shape1 First array shape
 * @param shape2 Second array shape
 * @return bool True if shapes are equal, false otherwise
 */
bool ptx_array_shape_equals(const ptx_array_shape_t shape1, const ptx_array_shape_t shape2);

#endif /* HEDRA_PTX_ARRAY_SHAPE_H */ 