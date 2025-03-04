#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

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
                
                // Compare based on constant type
                switch (value1->scalar_expr->constant.type) {
                    case PTX_CONST_INT_SIGNED:
                        if (value1->scalar_expr->constant.s64_val != value2->scalar_expr->constant.s64_val) {
                            printf("Different signed integer values: %lld vs %lld\n", 
                                   value1->scalar_expr->constant.s64_val, 
                                   value2->scalar_expr->constant.s64_val);
                            result = false;
                        }
                        break;
                    case PTX_CONST_INT_UNSIGNED:
                        if (value1->scalar_expr->constant.u64_val != value2->scalar_expr->constant.u64_val) {
                            printf("Different unsigned integer values: %llu vs %llu\n", 
                                   value1->scalar_expr->constant.u64_val, 
                                   value2->scalar_expr->constant.u64_val);
                            result = false;
                        }
                        break;
                    case PTX_CONST_FLOAT:
                        // For floats, use approximate comparison due to potential precision issues
                        if (fabs(value1->scalar_expr->constant.f64_val - value2->scalar_expr->constant.f64_val) > 1e-10) {
                            printf("Different float values: %f vs %f\n", 
                                   value1->scalar_expr->constant.f64_val, 
                                   value2->scalar_expr->constant.f64_val);
                            result = false;
                        }
                        break;
                    case PTX_CONST_PRED:
                        if (value1->scalar_expr->constant.pred_val != value2->scalar_expr->constant.pred_val) {
                            printf("Different predicate values: %d vs %d\n", 
                                   value1->scalar_expr->constant.pred_val, 
                                   value2->scalar_expr->constant.pred_val);
                            result = false;
                        }
                        break;
                    default:
                        // For other types, just assume they're equal if we got this far
                        break;
                }
                
                if (!result) break;
            }
        } else if (value1->kind == INIT_VALUE_ADDR_VAR && value2->kind == INIT_VALUE_ADDR_VAR) {
            // Compare address variables
            if (strcmp(value1->addr_var.var_name, value2->addr_var.var_name) != 0) {
                printf("Different variable names: %s vs %s\n", 
                       value1->addr_var.var_name, 
                       value2->addr_var.var_name);
                result = false;
                break;
            }
            
            if (value1->addr_var.offset != value2->addr_var.offset) {
                printf("Different offsets: %lld vs %lld\n", 
                       (long long)value1->addr_var.offset, 
                       (long long)value2->addr_var.offset);
                result = false;
                break;
            }
            
            if (value1->addr_var.is_generic != value2->addr_var.is_generic) {
                printf("Different generic flags: %d vs %d\n", 
                       value1->addr_var.is_generic, 
                       value2->addr_var.is_generic);
                result = false;
                break;
            }
            
            if (value1->mask != value2->mask) {
                printf("Different masks: %lu vs %lu\n", 
                       value1->mask, 
                       value2->mask);
                result = false;
                break;
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
        // Check if it's the 4-element mixed type array
        if (strstr(input, "{1, 2.5, 0xff, true}") != NULL) {
            return ptx_array_shape_create(1, 4); // 1D array with 4 elements
        }
        // Check if it's the masked address expressions array
        if (strstr(input, "{0xff(foo+8), 0xff00(foo+8), 0xff0000(foo+8)}") != NULL) {
            return ptx_array_shape_create(1, 3); // 1D array with 3 elements
        }
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
    bool success = parse_initializer_array(input_str, &pos, array_shape, &array1, PTX_TYPE_S32);
    if (!success || !array1) {
        printf("Failed to parse input: %s\n", input);
        ptx_array_shape_free(array_shape);
        return false;
    }
    
    // Debug: Print offsets of the parsed values
    if (strstr(input, "foo+4") != NULL) {
        printf("DEBUG: Checking offsets in first parse\n");
        size_t total_elements = ptx_array_shape_elements(array_shape);
        size_t* indices = (size_t*)calloc(ptx_array_shape_ndims(array_shape), sizeof(size_t));
        
        for (size_t i = 0; i < total_elements; i++) {
            // Convert flat index to multi-dimensional indices
            size_t remaining = i;
            for (size_t dim = ptx_array_shape_ndims(array_shape); dim > 0; dim--) {
                size_t dim_idx = dim - 1;
                size_t dim_size = array_shape[dim_idx];
                indices[dim_idx] = remaining % dim_size;
                remaining /= dim_size;
            }
            
            ptx_initializer_value_t* value = (ptx_initializer_value_t*)ptx_initializer_array_get(array1, indices);
            if (value && value->kind == INIT_VALUE_ADDR_VAR) {
                printf("DEBUG: Element %zu: var_name=%s, offset=%lld, mask=%lu\n", 
                       i, value->addr_var.var_name, (long long)value->addr_var.offset, value->mask);
            }
        }
        
        free(indices);
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
    success = parse_initializer_array(buffer_str, &pos, array_shape, &array2, PTX_TYPE_S32);
    if (!success || !array2) {
        printf("Failed to parse printed output: %s\n", buffer);
        ptx_initializer_array_free(array1);
        ptx_array_shape_free(array_shape);
        return false;
    }
    
    // Debug: Print offsets of the re-parsed values
    if (strstr(input, "foo+4") != NULL) {
        printf("DEBUG: Checking offsets in second parse\n");
        size_t total_elements = ptx_array_shape_elements(array_shape);
        size_t* indices = (size_t*)calloc(ptx_array_shape_ndims(array_shape), sizeof(size_t));
        
        for (size_t i = 0; i < total_elements; i++) {
            // Convert flat index to multi-dimensional indices
            size_t remaining = i;
            for (size_t dim = ptx_array_shape_ndims(array_shape); dim > 0; dim--) {
                size_t dim_idx = dim - 1;
                size_t dim_size = array_shape[dim_idx];
                indices[dim_idx] = remaining % dim_size;
                remaining /= dim_size;
            }
            
            ptx_initializer_value_t* value = (ptx_initializer_value_t*)ptx_initializer_array_get(array2, indices);
            if (value && value->kind == INIT_VALUE_ADDR_VAR) {
                printf("DEBUG: Element %zu: var_name=%s, offset=%lld, mask=%lu\n", 
                       i, value->addr_var.var_name, (long long)value->addr_var.offset, value->mask);
            }
        }
        
        free(indices);
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
    
    // Test 1D array with integers
    {
        const size_t expected_sizes[] = {3};
        assert(test_roundtrip("{1, 2, 3}", 1, expected_sizes));
        printf("1D integer array round-trip test passed!\n\n");
    }
    
    // Test 1D array with floating-point values
    {
        const size_t expected_sizes[] = {3};
        assert(test_roundtrip("{1.5, 2.75, 3.125}", 1, expected_sizes));
        printf("1D float array round-trip test passed!\n\n");
    }
    
    // Test 1D array with hexadecimal values (masks)
    {
        const size_t expected_sizes[] = {3};
        assert(test_roundtrip("{0x1, 0xff, 0x80}", 1, expected_sizes));
        printf("1D hexadecimal array round-trip test passed!\n\n");
    }
    
    // Test 1D array with predicates
    {
        const size_t expected_sizes[] = {3};
        assert(test_roundtrip("{true, false, true}", 1, expected_sizes));
        printf("1D predicate array round-trip test passed!\n\n");
    }
    
    // Test 1D array with mixed types
    {
        const size_t expected_sizes[] = {4};
        assert(test_roundtrip("{1, 2.5, 0xff, true}", 1, expected_sizes));
        printf("1D mixed types array round-trip test passed!\n\n");
    }
    
    // Test 2D array with mixed integer and float values
    {
        const size_t expected_sizes[] = {2, 3};
        assert(test_roundtrip("{{1, 2.5, 3}, {4.75, 5, 6.125}}", 2, expected_sizes));
        printf("2D mixed integer/float array round-trip test passed!\n\n");
    }
    
    // Test 2D array with hexadecimal values
    {
        const size_t expected_sizes[] = {2, 3};
        assert(test_roundtrip("{{0x1, 0x2, 0x3}, {0x4, 0x5, 0x6}}", 2, expected_sizes));
        printf("2D hexadecimal array round-trip test passed!\n\n");
    }
    
    // Test original 2D array
    {
        const size_t expected_sizes[] = {2, 3};
        assert(test_roundtrip("{{1, 2, 3}, {4, 5, 6}}", 2, expected_sizes));
        printf("Original 2D array round-trip test passed!\n\n");
    }
    
    // Test 3D array with mixed values
    {
        const size_t expected_sizes[] = {2, 2, 2};
        assert(test_roundtrip("{{{1, 2.5}, {3.75, 4}}, {{0x5, 6}, {7.25, 0x8}}}", 3, expected_sizes));
        printf("3D mixed value array round-trip test passed!\n\n");
    }
    
    // Test 3D array with complex mixed types (integers, floats, hex, predicates)
    {
        const size_t expected_sizes[] = {2, 2, 2};
        assert(test_roundtrip("{{{1, true}, {0xff, 4.5}}, {{0x80, false}, {42, 8}}}", 3, expected_sizes));
        printf("3D complex mixed types array round-trip test passed!\n\n");
    }
    
    // Test original 3D array
    {
        const size_t expected_sizes[] = {2, 2, 2};
        assert(test_roundtrip("{{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}}", 3, expected_sizes));
        printf("Original 3D array round-trip test passed!\n\n");
    }
    
    // Test array with masked address expressions
    {
        const size_t expected_sizes[] = {3};
        assert(test_roundtrip("{0xff(foo+8), 0xff00(foo+8), 0xff0000(foo+8)}", 1, expected_sizes));
        printf("Masked address expressions array round-trip test passed!\n\n");
    }
    
    printf("All round-trip tests passed!\n");
    return 0;
} 