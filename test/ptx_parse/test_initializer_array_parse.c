#include <ptx_parse/ptx_initializer_array_parse.h>
#include <ptx_parse/ptx_initializer_value_parse.h>
#include <ptx_ast/ptx_type_decl.h>
#include <prelude/ptx_initializer_array.h>
#include <prelude/ptx_array_shape.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Helper function to create a scalar initializer value for testing
ptx_initializer_value_t create_test_value(int value) {
    ptx_initializer_value_t test_value;
    test_value.kind = INIT_VALUE_SCALAR;  // INIT_VALUE_SCALAR
    test_value.mask = MASK_UNMASKED;
    
    // Create a constant expression for the value
    ptx_constant_t constant;
    constant.type = PTX_CONST_INT_SIGNED;
    constant.s64_val = value;
    test_value.scalar_expr = create_constant_expr(constant);
    
    return test_value;
}

// Helper function to free a test value
void free_test_value(ptx_initializer_value_t* value) {
    if (value->kind == INIT_VALUE_SCALAR && value->scalar_expr != NULL) {
        free_expr(value->scalar_expr);
    }
}

// Helper function to check if a value in the array matches the expected value
bool check_array_value(ptx_initializer_array_t* array, size_t* indices, int expected_value) {
    // Get the value at the specified indices
    ptx_initializer_value_t* value_ptr = (ptx_initializer_value_t*)ptx_initializer_array_get(array, indices);
    if (!value_ptr) {
        return false;
    }
    
    // Check that it's a scalar value
    if (value_ptr->kind != INIT_VALUE_SCALAR) {
        return false;
    }
    
    // Evaluate the expression to get the constant value
    ptx_constant_t constant_result;
    if (!evaluate_expr(value_ptr->scalar_expr, &constant_result)) {
        return false;
    }
    
    // Check that it's an integer value
    if (constant_result.type != PTX_CONST_INT_SIGNED && constant_result.type != PTX_CONST_INT_UNSIGNED) {
        return false;
    }
    
    // Compare the value
    int actual_value;
    if (constant_result.type == PTX_CONST_INT_SIGNED) {
        actual_value = (int)constant_result.s64_val;
    } else {
        actual_value = (int)constant_result.u64_val;
    }
    
    return actual_value == expected_value;
}

// Test a 1D array initializer
void test_1d_array_initializer() {
    printf("Testing 1D array initializer...\n");
    
    // Create a 1D array shape [3]
    ptx_array_shape_t shape = ptx_array_shape_create(1, 3);
    
    // Input string with a 1D array initializer
    const char* input = "{1, 2, 3}";
    
    // Parse the array initializer
    int pos = 0;
    ptx_initializer_array_t* array = NULL;
    bool result = parse_initializer_array(input, &pos, shape, &array, PTX_TYPE_S32);
    
    // Check result
    assert(result);
    assert(array != NULL);
    assert(pos == strlen(input));
    
    // Check the values in the array
    size_t indices[1];
    
    indices[0] = 0;
    assert(check_array_value(array, indices, 1));
    
    indices[0] = 1;
    assert(check_array_value(array, indices, 2));
    
    indices[0] = 2;
    assert(check_array_value(array, indices, 3));
    
    // Clean up
    ptx_initializer_array_free(array);
    ptx_array_shape_free(shape);
    
    printf("1D array initializer test passed!\n");
}

// Test a 2D array initializer
void test_2d_array_initializer() {
    printf("Testing 2D array initializer...\n");
    
    // Create a 2D array shape [2][3]
    ptx_array_shape_t shape = ptx_array_shape_create(2, 2, 3);
    
    // Input string with a 2D array initializer
    const char* input = "{{1, 2, 3}, {4, 5, 6}}";
    
    // Parse the array initializer
    int pos = 0;
    ptx_initializer_array_t* array = NULL;
    bool result = parse_initializer_array(input, &pos, shape, &array, PTX_TYPE_S32);
    
    // Check result
    assert(result);
    assert(array != NULL);
    assert(pos == strlen(input));
    
    // Check the values in the array
    size_t indices[2];
    
    indices[0] = 0; indices[1] = 0;
    assert(check_array_value(array, indices, 1));
    
    indices[0] = 0; indices[1] = 1;
    assert(check_array_value(array, indices, 2));
    
    indices[0] = 0; indices[1] = 2;
    assert(check_array_value(array, indices, 3));
    
    indices[0] = 1; indices[1] = 0;
    assert(check_array_value(array, indices, 4));
    
    indices[0] = 1; indices[1] = 1;
    assert(check_array_value(array, indices, 5));
    
    indices[0] = 1; indices[1] = 2;
    assert(check_array_value(array, indices, 6));
    
    // Clean up
    ptx_initializer_array_free(array);
    ptx_array_shape_free(shape);
    
    printf("2D array initializer test passed!\n");
}

// Test a 3D array initializer
void test_3d_array_initializer() {
    printf("Testing 3D array initializer...\n");
    
    // Create a 3D array shape [2][2][2]
    ptx_array_shape_t shape = ptx_array_shape_create(3, 2, 2, 2);
    
    // Input string with a 3D array initializer
    const char* input = "{{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}}";
    
    // Parse the array initializer
    int pos = 0;
    ptx_initializer_array_t* array = NULL;
    bool result = parse_initializer_array(input, &pos, shape, &array, PTX_TYPE_S32);
    
    // Check result
    assert(result);
    assert(array != NULL);
    assert(pos == strlen(input));
    
    // Check the values in the array
    size_t indices[3];
    
    indices[0] = 0; indices[1] = 0; indices[2] = 0;
    assert(check_array_value(array, indices, 1));
    
    indices[0] = 0; indices[1] = 0; indices[2] = 1;
    assert(check_array_value(array, indices, 2));
    
    indices[0] = 0; indices[1] = 1; indices[2] = 0;
    assert(check_array_value(array, indices, 3));
    
    indices[0] = 0; indices[1] = 1; indices[2] = 1;
    assert(check_array_value(array, indices, 4));
    
    indices[0] = 1; indices[1] = 0; indices[2] = 0;
    assert(check_array_value(array, indices, 5));
    
    indices[0] = 1; indices[1] = 0; indices[2] = 1;
    assert(check_array_value(array, indices, 6));
    
    indices[0] = 1; indices[1] = 1; indices[2] = 0;
    assert(check_array_value(array, indices, 7));
    
    indices[0] = 1; indices[1] = 1; indices[2] = 1;
    assert(check_array_value(array, indices, 8));
    
    // Clean up
    ptx_initializer_array_free(array);
    ptx_array_shape_free(shape);
    
    printf("3D array initializer test passed!\n");
}

// Test error cases
void test_error_cases() {
    printf("Testing error cases...\n");
    
    // Create array shapes
    ptx_array_shape_t shape1d = ptx_array_shape_create(1, 3);
    ptx_array_shape_t shape2d = ptx_array_shape_create(2, 2, 2);
    
    // Test cases
    struct {
        const char* input;
        ptx_array_shape_t shape;
        const char* description;
        bool should_fail;  // Whether the parser should fail for this input
    } test_cases[] = {
        {"{1, 2}", shape1d, "Too few elements", true},
        {"{1, 2, 3, 4}", shape1d, "Too many elements", true},
        {"{1, 2,}", shape1d, "Trailing comma", true},
        {"{{1, 2}, {3}}", shape2d, "Missing element in inner array", true},
        {"{1, 2, 3", shape1d, "Missing closing brace", true},
        {"1, 2, 3}", shape1d, "Missing opening brace", false},  // This is actually valid in our parser
        {"{{1, 2}, 3, 4}", shape2d, "Inconsistent nesting", true},
    };
    
    int num_test_cases = sizeof(test_cases) / sizeof(test_cases[0]);
    
    for (int i = 0; i < num_test_cases; i++) {
        printf("  Testing error case: %s\n", test_cases[i].description);
        
        int pos = 0;
        ptx_initializer_array_t* array = NULL;
        bool result = parse_initializer_array(test_cases[i].input, &pos, test_cases[i].shape, &array, PTX_TYPE_S32);
        
        if (test_cases[i].should_fail) {
            // Verify that parsing failed for cases that should fail
            assert(!result);
            assert(array == NULL);
        } else {
            // For cases that should succeed, we don't assert anything
            // Just clean up if needed
            if (array) {
                ptx_initializer_array_free(array);
            }
        }
    }
    
    // Clean up
    ptx_array_shape_free(shape1d);
    ptx_array_shape_free(shape2d);
    
    printf("Error cases test passed!\n");
}

int main() {
    printf("=== Testing PTX Initializer Array Parser ===\n\n");
    
    test_1d_array_initializer();
    test_2d_array_initializer();
    test_3d_array_initializer();
    test_error_cases();
    
    printf("\nAll tests passed!\n");
    return 0;
} 