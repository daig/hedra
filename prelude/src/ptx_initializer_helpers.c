/**
 * @file ptx_initializer_helpers.c
 * @brief Implementation of helper functions for working with PTX initializers
 */

#include <ptx_ast/>ptx_initializer_helpers.h>
#include <ptx_ast/>ptx_initializer_value_helpers.h>
#include <stdlib.h>
#include <string.h>

ptx_initializer_t ptx_initializer_create_scalar(ptx_initializer_kind_t kind, const ptx_initializer_value_t* value) {
    ptx_initializer_t initializer;
    initializer.kind = kind;
    
    if (value != NULL) {
        initializer.data.scalar = *value;
    } else {
        // Initialize to zero
        memset(&initializer.data.scalar, 0, sizeof(ptx_initializer_value_t));
        initializer.data.scalar.kind = kind;
    }
    
    return initializer;
}

ptx_initializer_t ptx_initializer_create_vector(ptx_vector_size_t size, const ptx_initializer_value_t* values) {
    ptx_initializer_t initializer;
    initializer.kind = INIT_VECTOR;
    
    // Create a 1D array shape with the vector size
    ptx_array_shape_t shape = ptx_array_shape_create(1, size);
    
    // Create the initializer array
    initializer.data.values = ptx_initializer_array_create(shape, sizeof(ptx_initializer_value_t));
    
    // Clean up the shape since it's been copied
    ptx_array_shape_free(shape);
    
    // If values were provided, copy them into the array
    if (values != NULL) {
        for (size_t i = 0; i < size; i++) {
            size_t indices[1] = {i};
            ptx_initializer_array_set(initializer.data.values, indices, &values[i]);
        }
    }
    
    return initializer;
}

ptx_initializer_t ptx_initializer_create_array(const ptx_array_shape_t shape) {
    ptx_initializer_t initializer;
    initializer.kind = INIT_ARRAY;
    
    // Create the initializer array
    initializer.data.values = ptx_initializer_array_create(shape, sizeof(ptx_initializer_value_t));
    
    return initializer;
}

int ptx_initializer_set_value(ptx_initializer_t* initializer, const size_t* indices, const ptx_initializer_value_t* value) {
    if (initializer == NULL || indices == NULL || value == NULL) {
        return -1;
    }
    
    if (initializer->kind != INIT_VECTOR && initializer->kind != INIT_ARRAY) {
        return -1;  // Not a vector or array
    }
    
    return ptx_initializer_array_set(initializer->data.values, indices, value);
}

int ptx_initializer_get_value(const ptx_initializer_t* initializer, const size_t* indices, ptx_initializer_value_t* value) {
    if (initializer == NULL || indices == NULL || value == NULL) {
        return -1;
    }
    
    if (initializer->kind != INIT_VECTOR && initializer->kind != INIT_ARRAY) {
        return -1;  // Not a vector or array
    }
    
    void* ptr = ptx_initializer_array_get(initializer->data.values, indices);
    if (ptr == NULL) {
        return -1;
    }
    
    *value = *(ptx_initializer_value_t*)ptr;
    return 0;
}

void ptx_initializer_free(ptx_initializer_t* initializer) {
    if (initializer == NULL) {
        return;
    }
    
    if ((initializer->kind == INIT_VECTOR || initializer->kind == INIT_ARRAY) && 
        initializer->data.values != NULL) {
        ptx_initializer_array_free(initializer->data.values);
        initializer->data.values = NULL;
    }
    
    // Reset the initializer to INIT_NONE
    initializer->kind = INIT_NONE;
}

ptx_initializer_t ptx_initializer_create_int(int64_t value) {
    ptx_initializer_value_t val = ptx_initializer_value_int(value);
    return ptx_initializer_create_scalar(INIT_SCALAR_INT, &val);
}

ptx_initializer_t ptx_initializer_create_float(double value) {
    ptx_initializer_value_t val = ptx_initializer_value_float(value);
    return ptx_initializer_create_scalar(INIT_SCALAR_FLOAT, &val);
}

ptx_initializer_t ptx_initializer_create_var_addr(const char* var_name, int64_t offset, bool is_generic) {
    ptx_initializer_value_t val = ptx_initializer_value_var_addr(var_name, offset, is_generic);
    return ptx_initializer_create_scalar(INIT_VAR_ADDR, &val);
}

ptx_initializer_t ptx_initializer_create_func_addr(const char* func_name) {
    ptx_initializer_value_t val = ptx_initializer_value_func_addr(func_name);
    return ptx_initializer_create_scalar(INIT_FUNC_ADDR, &val);
}

ptx_initializer_t ptx_initializer_create_masked_var_addr(const char* var_name, int64_t offset, bool is_generic, ptx_mask_value_t mask) {
    ptx_initializer_value_t val = ptx_initializer_value_masked_var_addr(var_name, offset, is_generic, mask);
    return ptx_initializer_create_scalar(INIT_MASKED_VAR_ADDR, &val);
}

ptx_initializer_t ptx_initializer_create_masked_int(int64_t value, ptx_mask_value_t mask) {
    ptx_initializer_value_t val = ptx_initializer_value_masked_int(value, mask);
    return ptx_initializer_create_scalar(INIT_MASKED_INT, &val);
} 