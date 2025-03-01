#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include <ptx_parse/ptx_initializer_array_parse.h>
#include <ptx_print/ptx_initializer_array_print.h>
#include <prelude/ptx_initializer_array.h>
#include <prelude/ptx_array_shape.h>
#include <ptx_ast/ptx_constant_expr.h>
#include <ptx_ast/ptx_constant.h>

// Helper function to compare two initializer arrays
static bool compare_initializer_arrays(const ptx_initializer_array_t* array1, const ptx_initializer_array_t* array2) {
    if (!array1 || !array2) {
        return false;
    }
    
    // Compare shapes
    ptx_array_shape_t shape1 = ptx_initializer_array_shape(array1);
    ptx_array_shape_t shape2 = ptx_initializer_array_shape(array2);
    
    if (!shape1 || !shape2) {
        return false;
    }
    
    size_t ndims1 = ptx_array_shape_ndims(shape1);
    size_t ndims2 = ptx_array_shape_ndims(shape2);
    
    if (ndims1 != ndims2) {
        printf("Different number of dimensions: %zu vs %zu\n", ndims1, ndims2);
        return false;
    }
    
    // Compare dimensions
    for (size_t i = 0; i < ndims1; i++) {
        if (shape1[i] != shape2[i]) {
            printf("Different dimension size at index %zu: %zu vs %zu\n", i, shape1[i], shape2[i]);
            return false;
        }
    }
    
    // Compare all elements
    size_t total_elements = ptx_array_shape_elements(shape1);
    size_t* indices = (size_t*)calloc(ndims1, sizeof(size_t));
    if (!indices) {
        return false;
    }
    
    bool result = true;
    
    // Iterate through all elements using a flat index
    for (size_t flat_idx = 0; flat_idx < total_elements; flat_idx++) {
        // Convert flat index to multi-dimensional indices
        size_t remaining = flat_idx;
        for (size_t dim = ndims1; dim > 0; dim--) {
            size_t dim_idx = dim - 1;
            size_t dim_size = shape1[dim_idx];
            indices[dim_idx] = remaining % dim_size;
            remaining /= dim_size;
        }
        
        // Get elements at the current indices
        ptx_initializer_value_t* value1 = (ptx_initializer_value_t*)ptx_initializer_array_get(
            (ptx_initializer_array_t*)array1, indices);
        ptx_initializer_value_t* value2 = (ptx_initializer_value_t*)ptx_initializer_array_get(
            (ptx_initializer_array_t*)array2, indices);
        
        // Compare elements
        if (!value1 && !value2) {
            continue; // Both are NULL, which is fine
        }
        
        if (!value1 || !value2) {
            printf("One value is NULL at index [");
            for (size_t i = 0; i < ndims1; i++) {
                printf("%zu", indices[i]);
                if (i < ndims1 - 1) printf(", ");
            }
            printf("]\n");
            result = false;
            break;
        }
        
        // For simplicity, we'll just check if they're the same kind and have the same scalar value
        // A more thorough comparison would need to compare the actual expressions
        if (value1->kind != value2->kind) {
            printf("Different kinds at index [");
            for (size_t i = 0; i < ndims1; i++) {
                printf("%zu", indices[i]);
                if (i < ndims1 - 1) printf(", ");
            }
            printf("]: %d vs %d\n", value1->kind, value2->kind);
            result = false;
            break;
        }
        
        // If they're scalar values, compare the expressions
        // This is a simplified comparison that assumes constant expressions
        if (value1->kind == INIT_VALUE_SCALAR && value2->kind == INIT_VALUE_SCALAR) {
            if (value1->scalar_expr->type != value2->scalar_expr->type) {
                printf("Different expression types\n");
                result = false;
                break;
            }
            
            // For constant expressions, compare the values
            if (value1->scalar_expr->type == EXPR_CONSTANT && value2->scalar_expr->type == EXPR_CONSTANT) {
                if (value1->scalar_expr->constant.type != value2->scalar_expr->constant.type) {
                    printf("Different constant types\n");
                    result = false;
                    break;
                }
                
                // For integer constants, compare the values
                if (value1->scalar_expr->constant.type == PTX_CONST_INT_SIGNED) {
                    if (value1->scalar_expr->constant.s64_val != value2->scalar_expr->constant.s64_val) {
                        printf("Different integer values: %lld vs %lld\n", 
                               value1->scalar_expr->constant.s64_val, 
                               value2->scalar_expr->constant.s64_val);
                        result = false;
                        break;
                    }
                }
                // Add more comparisons for other constant types if needed
            }
        }
    }
    
    free(indices);
    return result;
}

// Helper function to parse an array shape from a string
static ptx_array_shape_t parse_array_shape(const char* input, size_t expected_dims) {
    // For simplicity, we'll just use the expected sizes directly
    if (expected_dims == 1) {
        return ptx_array_shape_create(1, 3); // 1D array with 3 elements
    } else if (expected_dims == 2) {
        return ptx_array_shape_create(2, 2, 3); // 2D array with 2x3 elements
    } else if (expected_dims == 3) {
        return ptx_array_shape_create(3, 2, 2, 2); // 3D array with 2x2x2 elements
    }
    
    return NULL;
}

// Test function for round-trip parsing and printing
static bool test_roundtrip(const char* input, size_t expected_dims, const size_t* expected_sizes) {
    printf("Testing round-trip for: %s\n", input);
    
    // Create array shape
    ptx_array_shape_t array_shape = parse_array_shape(input, expected_dims);
    if (!array_shape) {
        printf("Failed to create array shape\n");
        return false;
    }
    
    // First parse
    const char* input_str = input;
    int pos = 0;
    ptx_initializer_array_t* array1 = NULL;
    bool success = parse_initializer_array(input_str, &pos, array_shape, &array1);
    if (!success || !array1) {
        printf("Failed to parse input: %s\n", input);
        ptx_array_shape_free(array_shape);
        return false;
    }
    
    // Print to string
    char buffer[1024];
    int printed = sprint_ptx_initializer_array(buffer, sizeof(buffer), array1);
    if (printed < 0) {
        printf("Failed to print array\n");
        ptx_initializer_array_free(array1);
        ptx_array_shape_free(array_shape);
        return false;
    }
    
    printf("Printed array: %s\n", buffer);
    
    // Parse the printed string
    const char* buffer_str = buffer;
    pos = 0;
    ptx_initializer_array_t* array2 = NULL;
    success = parse_initializer_array(buffer_str, &pos, array_shape, &array2);
    if (!success || !array2) {
        printf("Failed to parse printed output: %s\n", buffer);
        ptx_initializer_array_free(array1);
        ptx_array_shape_free(array_shape);
        return false;
    }
    
    // Compare the two arrays
    bool result = compare_initializer_arrays(array1, array2);
    
    // Verify dimensions
    if (result) {
        ptx_array_shape_t shape = ptx_initializer_array_shape(array1);
        size_t ndims = ptx_array_shape_ndims(shape);
        
        if (ndims != expected_dims) {
            printf("Expected %zu dimensions, got %zu\n", expected_dims, ndims);
            result = false;
        } else {
            for (size_t i = 0; i < ndims; i++) {
                if (shape[i] != expected_sizes[i]) {
                    printf("Expected dimension %zu to be %zu, got %zu\n", i, expected_sizes[i], shape[i]);
                    result = false;
                    break;
                }
            }
        }
    }
    
    // Clean up
    ptx_initializer_array_free(array1);
    ptx_initializer_array_free(array2);
    ptx_array_shape_free(array_shape);
    
    return result;
}

int main() {
    printf("=== Testing PTX Initializer Array Round-Trip ===\n\n");
    
    // Test 1D array
    {
        const size_t expected_sizes[] = {3};
        assert(test_roundtrip("{1, 2, 3}", 1, expected_sizes));
        printf("1D array round-trip test passed!\n\n");
    }
    
    // Test 2D array
    {
        const size_t expected_sizes[] = {2, 3};
        assert(test_roundtrip("{{1, 2, 3}, {4, 5, 6}}", 2, expected_sizes));
        printf("2D array round-trip test passed!\n\n");
    }
    
    // Test 3D array
    {
        const size_t expected_sizes[] = {2, 2, 2};
        assert(test_roundtrip("{{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}}", 3, expected_sizes));
        printf("3D array round-trip test passed!\n\n");
    }
    
    printf("All round-trip tests passed!\n");
    return 0;
} 