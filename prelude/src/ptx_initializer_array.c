/**
 * @file ptx_initializer_array.c
 * @brief Implementation of dynamic multi-dimensional array using ptx_array_shape_t
 */

#include <prelude/>ptx_initializer_array.h>
#include <stdlib.h>
#include <string.h>

ptx_initializer_array_t* ptx_initializer_array_create(const ptx_array_shape_t shape, size_t element_size) {
    if (shape == NULL || element_size == 0) {
        return NULL;
    }

    ptx_initializer_array_t* array = (ptx_initializer_array_t*)malloc(sizeof(ptx_initializer_array_t));
    if (array == NULL) {
        return NULL;
    }

    // Copy the shape
    array->shape = ptx_array_shape_copy(shape);
    if (array->shape == NULL) {
        free(array);
        return NULL;
    }

    array->element_size = element_size;
    
    // Calculate total number of elements
    size_t total_elements = ptx_array_shape_elements(shape);
    
    // Allocate data array
    if (total_elements > 0) {
        array->data = malloc(total_elements * element_size);
        if (array->data == NULL) {
            ptx_array_shape_free(array->shape);
            free(array);
            return NULL;
        }
        // Initialize to zero
        memset(array->data, 0, total_elements * element_size);
    } else {
        array->data = NULL;
    }

    return array;
}

void ptx_initializer_array_free(ptx_initializer_array_t* array) {
    if (array == NULL) {
        return;
    }
    
    if (array->shape != NULL) {
        ptx_array_shape_free(array->shape);
    }
    
    if (array->data != NULL) {
        free(array->data);
    }
    
    free(array);
}

// Helper function to calculate linear index from multi-dimensional indices
static size_t calculate_linear_index(const ptx_array_shape_t shape, const size_t* indices) {
    size_t ndims = ptx_array_shape_ndims(shape);
    size_t linear_index = 0;
    size_t stride = 1;
    
    // Calculate in reverse order (row-major)
    for (int i = ndims - 1; i >= 0; i--) {
        linear_index += indices[i] * stride;
        stride *= shape[i];
    }
    
    return linear_index;
}

// Helper function to check if indices are within bounds
static bool indices_in_bounds(const ptx_array_shape_t shape, const size_t* indices) {
    size_t i = 0;
    while (shape[i] != 0) {
        if (indices[i] >= shape[i]) {
            return false;
        }
        i++;
    }
    return true;
}

void* ptx_initializer_array_get(const ptx_initializer_array_t* array, const size_t* indices) {
    if (array == NULL || array->data == NULL || indices == NULL) {
        return NULL;
    }
    
    if (!indices_in_bounds(array->shape, indices)) {
        return NULL;
    }
    
    size_t linear_index = calculate_linear_index(array->shape, indices);
    return (char*)array->data + (linear_index * array->element_size);
}

int ptx_initializer_array_set(ptx_initializer_array_t* array, const size_t* indices, const void* value) {
    if (array == NULL || array->data == NULL || indices == NULL || value == NULL) {
        return -1;
    }
    
    if (!indices_in_bounds(array->shape, indices)) {
        return -1;
    }
    
    size_t linear_index = calculate_linear_index(array->shape, indices);
    void* dest = (char*)array->data + (linear_index * array->element_size);
    memcpy(dest, value, array->element_size);
    
    return 0;
}

size_t ptx_initializer_array_elements(const ptx_initializer_array_t* array) {
    if (array == NULL || array->shape == NULL) {
        return 0;
    }
    
    return ptx_array_shape_elements(array->shape);
}

const ptx_array_shape_t ptx_initializer_array_shape(const ptx_initializer_array_t* array) {
    if (array == NULL) {
        return NULL;
    }
    
    return array->shape;
}

int ptx_initializer_array_resize(ptx_initializer_array_t* array, const ptx_array_shape_t new_shape) {
    if (array == NULL || new_shape == NULL) {
        return -1;
    }
    
    // Copy the new shape
    ptx_array_shape_t shape_copy = ptx_array_shape_copy(new_shape);
    if (shape_copy == NULL) {
        return -1;
    }
    
    // Calculate new total elements
    size_t new_total_elements = ptx_array_shape_elements(new_shape);
    
    // Allocate new data array
    void* new_data = NULL;
    if (new_total_elements > 0) {
        new_data = malloc(new_total_elements * array->element_size);
        if (new_data == NULL) {
            ptx_array_shape_free(shape_copy);
            return -1;
        }
        // Initialize to zero
        memset(new_data, 0, new_total_elements * array->element_size);
    }
    
    // Free old resources
    if (array->data != NULL) {
        free(array->data);
    }
    
    if (array->shape != NULL) {
        ptx_array_shape_free(array->shape);
    }
    
    // Update array with new resources
    array->shape = shape_copy;
    array->data = new_data;
    
    return 0;
} 