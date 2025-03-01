#include <ptx_parse/ptx_type_decl_parse.h>
#include <ptx_print/ptx_declaration_lhs_print.h>
#include <ptx_ast/ptx_type_decl.h>
#include <prelude/ptx_array_shape.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

// Helper function to print a scalar initializer value for debugging
static void print_initializer_value(ptx_initializer_value_t* value) {
    if (!value) {
        printf("  [NULL value]\n");
        return;
    }
    
    if (value->kind == INIT_VALUE_SCALAR) {
        printf("  [Scalar Expression] ");
        if (value->scalar_expr) {
            // Evaluate the expression to get the value
            ptx_constant_t result;
            if (evaluate_expr(value->scalar_expr, &result)) {
                switch (result.type) {
                    case PTX_CONST_INT_SIGNED:
                        printf("Value: %lld\n", (long long)result.s64_val);
                        break;
                    case PTX_CONST_INT_UNSIGNED:
                        printf("Value: %llu\n", (unsigned long long)result.u64_val);
                        break;
                    case PTX_CONST_FLOAT:
                        printf("Value: %f\n", result.f32_val);
                        break;
                    default:
                        printf("Value: [unknown type]\n");
                        break;
                }
            } else {
                printf("Value: [could not evaluate]\n");
            }
        } else {
            printf("Value: [NULL expression]\n");
        }
    } else if (value->kind == INIT_VALUE_ADDR_VAR) {
        printf("  [Address of Variable] %s", value->addr_var.var_name);
        if (value->addr_var.offset != 0) {
            printf(" + %lld", (long long)value->addr_var.offset);
        }
        printf("\n");
    } else {
        printf("  [Unknown value kind]\n");
    }
}

// Helper function to print an array initializer for debugging
static void print_array_initializer(ptx_initializer_array_t* array, ptx_array_shape_t shape, size_t dim_index, size_t* indices) {
    if (!array || !shape) {
        return;
    }
    
    size_t ndims = ptx_array_shape_ndims(shape);
    
    // Base case: at the innermost dimension, print the value
    if (dim_index >= ndims) {
        ptx_initializer_value_t* value = (ptx_initializer_value_t*)ptx_initializer_array_get(array, indices);
        if (value) {
            // Print indices
            printf("  [");
            for (size_t i = 0; i < ndims; i++) {
                printf("%zu", indices[i]);
                if (i < ndims - 1) {
                    printf(", ");
                }
            }
            printf("] = ");
            print_initializer_value(value);
        }
        return;
    }
    
    // Recursive case: iterate over the current dimension
    for (size_t i = 0; i < shape[dim_index]; i++) {
        indices[dim_index] = i;
        print_array_initializer(array, shape, dim_index + 1, indices);
    }
}

// Test basic scalar declaration without initializer
static void test_scalar_declaration_no_initializer() {
    printf("\n=== Testing scalar declaration without initializer ===\n");
    
    const char* test_str = ".global .u32 my_var";
    ptx_decl_t decl;
    
    printf("Parsing: \"%s\"\n", test_str);
    bool result = parse_ptx_decl(test_str, &decl);
    printf("Parse result: %s\n", result ? "SUCCESS" : "FAILED");
    assert(result && "Parsing failed");
    
    // Verify the declaration
    assert(decl.type.statespace == PTX_STATE_GLOBAL);
    assert(decl.type.type == PTX_TYPE_U32);
    assert(decl.type.shape.kind == SHAPE_SCALAR);
    assert(strcmp(decl.type.name, "my_var") == 0);
    assert(decl.type.has_initializer == false);
    
    printf("Declaration verified successfully\n");
    
    // Free resources
    free_ptx_decl(&decl);
}

// Test scalar declaration with initializer
static void test_scalar_declaration_with_initializer() {
    printf("\n=== Testing scalar declaration with initializer ===\n");
    
    const char* test_str = ".global .u32 my_var = 42";
    ptx_decl_t decl;
    
    printf("Parsing: \"%s\"\n", test_str);
    bool result = parse_ptx_decl(test_str, &decl);
    printf("Parse result: %s\n", result ? "SUCCESS" : "FAILED");
    assert(result && "Parsing failed");
    
    // Verify the declaration
    assert(decl.type.statespace == PTX_STATE_GLOBAL);
    assert(decl.type.type == PTX_TYPE_U32);
    assert(decl.type.shape.kind == SHAPE_SCALAR);
    assert(strcmp(decl.type.name, "my_var") == 0);
    assert(decl.type.has_initializer == true);
    assert(decl.scalar != NULL);
    assert(decl.scalar->kind == INIT_VALUE_SCALAR);
    
    // Evaluate the initializer expression
    ptx_constant_t result_value;
    bool eval_result = evaluate_expr(decl.scalar->scalar_expr, &result_value);
    assert(eval_result && "Failed to evaluate expression");
    assert(result_value.type == PTX_CONST_INT_SIGNED || result_value.type == PTX_CONST_INT_UNSIGNED);
    
    int64_t value = (result_value.type == PTX_CONST_INT_SIGNED) ? 
                    result_value.s64_val : (int64_t)result_value.u64_val;
    printf("Initializer value: %lld\n", (long long)value);
    assert(value == 42 && "Initializer value is incorrect");
    
    printf("Declaration verified successfully\n");
    
    // Free resources
    free_ptx_decl(&decl);
}

// Test array declaration with initializer
static void test_array_declaration_with_initializer() {
    printf("\n=== Testing array declaration with initializer ===\n");
    
    const char* test_str = ".global .u32 my_array[2][3] = {{1, 2, 3}, {4, 5, 6}}";
    ptx_decl_t decl;
    
    printf("Parsing: \"%s\"\n", test_str);
    bool result = parse_ptx_decl(test_str, &decl);
    printf("Parse result: %s\n", result ? "SUCCESS" : "FAILED");
    assert(result && "Parsing failed");
    
    // Verify the declaration
    assert(decl.type.statespace == PTX_STATE_GLOBAL);
    assert(decl.type.type == PTX_TYPE_U32);
    assert(decl.type.shape.kind == SHAPE_ARRAY);
    assert(strcmp(decl.type.name, "my_array") == 0);
    assert(decl.type.has_initializer == true);
    assert(decl.array != NULL);
    
    // Check array dimensions
    size_t ndims = ptx_array_shape_ndims(decl.type.shape.array_shape);
    printf("Array dimensions: %zu\n", ndims);
    assert(ndims == 2 && "Wrong number of dimensions");
    assert(decl.type.shape.array_shape[0] == 2 && "First dimension size is wrong");
    assert(decl.type.shape.array_shape[1] == 3 && "Second dimension size is wrong");
    
    // Print the array initializer for debugging
    printf("Array initializer values:\n");
    size_t* indices = (size_t*)calloc(ndims, sizeof(size_t));
    if (indices) {
        print_array_initializer(decl.array, decl.type.shape.array_shape, 0, indices);
        free(indices);
    }
    
    // Free resources
    free_ptx_decl(&decl);
    
    printf("Declaration verified successfully\n");
}

// Test array declaration without initializer
static void test_array_declaration_no_initializer() {
    printf("\n=== Testing array declaration without initializer ===\n");
    
    const char* test_str = ".global .u32 my_array[2][3]";
    ptx_decl_t decl;
    
    printf("Parsing: \"%s\"\n", test_str);
    bool result = parse_ptx_decl(test_str, &decl);
    printf("Parse result: %s\n", result ? "SUCCESS" : "FAILED");
    assert(result && "Parsing failed");
    
    // Verify the declaration
    assert(decl.type.statespace == PTX_STATE_GLOBAL);
    assert(decl.type.type == PTX_TYPE_U32);
    assert(decl.type.shape.kind == SHAPE_ARRAY);
    assert(strcmp(decl.type.name, "my_array") == 0);
    assert(decl.type.has_initializer == false);
    
    // Check array dimensions
    size_t ndims = ptx_array_shape_ndims(decl.type.shape.array_shape);
    printf("Array dimensions: %zu\n", ndims);
    assert(ndims == 2 && "Wrong number of dimensions");
    assert(decl.type.shape.array_shape[0] == 2 && "First dimension size is wrong");
    assert(decl.type.shape.array_shape[1] == 3 && "Second dimension size is wrong");
    
    printf("Declaration verified successfully\n");
    
    // Free resources
    free_ptx_decl(&decl);
}

// Test vector declaration with initializer
static void test_vector_declaration_with_initializer() {
    printf("\n=== Testing vector declaration with initializer ===\n");
    
    const char* test_str = ".global .v4 .f32 my_vector = {1.0, 2.0, 3.0, 4.0}";
    ptx_decl_t decl;
    
    printf("Parsing: \"%s\"\n", test_str);
    bool result = parse_ptx_decl(test_str, &decl);
    printf("Parse result: %s\n", result ? "SUCCESS" : "FAILED");
    assert(result && "Parsing failed");
    
    // Verify the declaration
    assert(decl.type.statespace == PTX_STATE_GLOBAL);
    assert(decl.type.type == PTX_TYPE_F32);
    assert(decl.type.shape.kind == SHAPE_VECTOR);
    assert(decl.type.shape.vector_size == VECTOR_SIZE_V4);
    assert(strcmp(decl.type.name, "my_vector") == 0);
    assert(decl.type.has_initializer == true);
    
    // Verify the initializer array exists
    assert(decl.array != NULL);
    
    // Check vector elements
    printf("Vector initializer values:\n");
    size_t indices[1];
    
    // Check each element of the vector
    for (size_t i = 0; i < 4; i++) {
        indices[0] = i;
        ptx_initializer_value_t* value = (ptx_initializer_value_t*)ptx_initializer_array_get(decl.array, indices);
        assert(value != NULL && "Vector initializer element not found");
        assert(value->kind == INIT_VALUE_SCALAR && "Vector initializer element is not a scalar");
        
        // Evaluate the initializer expression
        ptx_constant_t result_value;
        bool eval_result = evaluate_expr(value->scalar_expr, &result_value);
        assert(eval_result && "Failed to evaluate expression");
        assert(result_value.type == PTX_CONST_FLOAT_SINGLE && "Vector initializer element is not a float");
        
        float expected_value = (float)(i + 1.0);
        printf("  [%zu] = [Scalar Expression] Value: %f (Expected: %f)\n", 
               i, result_value.f32_val, expected_value);
        assert(fabs(result_value.f32_val - expected_value) < 0.0001f && 
               "Vector initializer value is incorrect");
    }
    
    printf("Declaration verified successfully\n");
    
    // Free resources
    free_ptx_decl(&decl);
    
    // Also test a v2 vector to ensure both sizes work
    printf("\nTesting .v2 vector declaration:\n");
    const char* test_str_v2 = ".global .v2 .f32 my_v2_vector = {5.0, 6.0}";
    
    printf("Parsing: \"%s\"\n", test_str_v2);
    result = parse_ptx_decl(test_str_v2, &decl);
    printf("Parse result: %s\n", result ? "SUCCESS" : "FAILED");
    assert(result && "Parsing v2 vector failed");
    
    // Verify the declaration
    assert(decl.type.statespace == PTX_STATE_GLOBAL);
    assert(decl.type.type == PTX_TYPE_F32);
    assert(decl.type.shape.kind == SHAPE_VECTOR);
    assert(decl.type.shape.vector_size == VECTOR_SIZE_V2);
    assert(strcmp(decl.type.name, "my_v2_vector") == 0);
    assert(decl.type.has_initializer == true);
    
    // Verify the initializer array exists
    assert(decl.array != NULL);
    
    // Check vector elements for v2
    printf("V2 vector initializer values:\n");
    
    // Check each element of the v2 vector
    for (size_t i = 0; i < 2; i++) {
        indices[0] = i;
        ptx_initializer_value_t* value = (ptx_initializer_value_t*)ptx_initializer_array_get(decl.array, indices);
        assert(value != NULL && "Vector initializer element not found");
        assert(value->kind == INIT_VALUE_SCALAR && "Vector initializer element is not a scalar");
        
        // Evaluate the initializer expression
        ptx_constant_t result_value;
        bool eval_result = evaluate_expr(value->scalar_expr, &result_value);
        assert(eval_result && "Failed to evaluate expression");
        assert(result_value.type == PTX_CONST_FLOAT_SINGLE && "Vector initializer element is not a float");
        
        float expected_value = (float)(i + 5.0);
        printf("  [%zu] = [Scalar Expression] Value: %f (Expected: %f)\n", 
               i, result_value.f32_val, expected_value);
        assert(fabs(result_value.f32_val - expected_value) < 0.0001f && 
               "Vector initializer value is incorrect");
    }
    
    printf("V2 declaration verified successfully\n");
    
    // Free resources
    free_ptx_decl(&decl);
}

// Main test function
int main() {
    printf("=== PTX Declaration Parser Tests ===\n");
    
    // Run the tests
    test_scalar_declaration_no_initializer();
    test_scalar_declaration_with_initializer();
    test_array_declaration_no_initializer();
    test_array_declaration_with_initializer();
    test_vector_declaration_with_initializer();
    
    printf("\nAll tests passed successfully!\n");
    return 0;
} 