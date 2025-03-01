/**
 * @file ptx_array_shape.c
 * @brief Implementation of array shape functions for multi-dimensional arrays
 */

#include "ptx_array_shape.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

ptx_array_shape_t ptx_array_shape_create(size_t dims, ...) {
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

ptx_array_shape_t ptx_array_shape_copy(const ptx_array_shape_t shape) {
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

size_t ptx_array_shape_ndims(const ptx_array_shape_t shape) {
    if (shape == NULL) {
        return 0;
    }

    size_t dims = 0;
    while (shape[dims] != 0) {
        dims++;
    }
    return dims;
}

void ptx_array_shape_free(ptx_array_shape_t shape) {
    free(shape);
}

size_t ptx_array_shape_elements(const ptx_array_shape_t shape) {
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

bool ptx_array_shape_equals(const ptx_array_shape_t shape1, const ptx_array_shape_t shape2) {
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