/**
 * @file prelude.h
 * @brief Header-only utility types and functions for the project
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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
 * @param dims Variable number of dimensions (must end with 0)
 * @return ptx_array_shape_t Newly allocated array shape (caller must free)
 */
static inline ptx_array_shape_t ptx_array_shape_create(size_t dims, ...) {
    if (dims == 0) {
        ptx_array_shape_t shape = (ptx_array_shape_t)malloc(sizeof(size_t));
        if (shape == NULL) {
            return NULL;
        }
        shape[0] = 0;
        return shape;
    }

    va_list args;
    va_start(args, dims);

    ptx_array_shape_t shape = (ptx_array_shape_t)malloc((dims + 1) * sizeof(size_t));
    if (shape == NULL) {
        va_end(args);
        return NULL;
    }

    for (size_t i = 0; i < dims; i++) {
        shape[i] = va_arg(args, size_t);
    }
    shape[dims] = 0; // Null terminator

    va_end(args);
    return shape;
}

/**
 * @brief Copy an array shape
 * 
 * @param shape Source shape to copy
 * @return ptx_array_shape_t New copy of the shape (caller must free)
 */
static inline ptx_array_shape_t ptx_array_shape_copy(const ptx_array_shape_t shape) {
    if (shape == NULL) {
        return NULL;
    }

    // Count dimensions
    size_t dims = 0;
    while (shape[dims] != 0) {
        dims++;
    }

    // Allocate and copy
    ptx_array_shape_t new_shape = (ptx_array_shape_t)malloc((dims + 1) * sizeof(size_t));
    if (new_shape == NULL) {
        return NULL;
    }

    memcpy(new_shape, shape, (dims + 1) * sizeof(size_t));
    return new_shape;
}

/**
 * @brief Get the number of dimensions in an array shape
 * 
 * @param shape The array shape
 * @return size_t Number of dimensions
 */
static inline size_t ptx_array_shape_ndims(const ptx_array_shape_t shape) {
    if (shape == NULL) {
        return 0;
    }

    size_t dims = 0;
    while (shape[dims] != 0) {
        dims++;
    }
    return dims;
}

/**
 * @brief Free an array shape
 * 
 * @param shape The array shape to free
 */
static inline void ptx_array_shape_free(ptx_array_shape_t shape) {
    free(shape);
}

/**
 * @brief Calculate the total number of elements in an array with the given shape
 * 
 * @param shape The array shape
 * @return size_t Total number of elements
 */
static inline size_t ptx_array_shape_elements(const ptx_array_shape_t shape) {
    if (shape == NULL || shape[0] == 0) {
        return 0;
    }

    size_t total = 1;
    size_t i = 0;
    while (shape[i] != 0) {
        total *= shape[i];
        i++;
    }
    return total;
}

/**
 * @brief Compare two array shapes for equality
 * 
 * @param shape1 First array shape
 * @param shape2 Second array shape
 * @return bool True if shapes are equal, false otherwise
 */
static inline bool ptx_array_shape_equals(const ptx_array_shape_t shape1, const ptx_array_shape_t shape2) {
    if (shape1 == NULL && shape2 == NULL) {
        return true;
    }
    if (shape1 == NULL || shape2 == NULL) {
        return false;
    }

    size_t i = 0;
    while (true) {
        if (shape1[i] != shape2[i]) {
            return false;
        }
        if (shape1[i] == 0) { // Both are zero here (from previous check)
            break;
        }
        i++;
    }
    return true;
}