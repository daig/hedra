#include <ptx_parse/ptx_declaration_lhs_parse.h>
#include <ptx_print/ptx_declaration_lhs_print.h>
#include <prelude/ptx_array_shape.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 256

/**
 * Helper function to cleanup a declaration LHS structure
 */
static void cleanup_decl_lhs(ptx_declaration_type_t *lhs) {
    if (lhs) {
        free(lhs->name);
        lhs->name = NULL;
        
        if (lhs->shape.kind == SHAPE_ARRAY && lhs->shape.array_shape) {
            ptx_array_shape_free(lhs->shape.array_shape);
            lhs->shape.array_shape = NULL;
        }
    }
}

/**
 * Debug function to print state space name
 */
static const char* get_state_space_name(ptx_state_space_t space) {
    switch (space) {
        case PTX_STATE_REG: return "PTX_STATE_REG";
        case PTX_STATE_SREG: return "PTX_STATE_SREG";
        case PTX_STATE_CONST: return "PTX_STATE_CONST";
        case PTX_STATE_GLOBAL: return "PTX_STATE_GLOBAL";
        case PTX_STATE_LOCAL: return "PTX_STATE_LOCAL";
        case PTX_STATE_PARAM: return "PTX_STATE_PARAM";
        case PTX_STATE_SHARED: return "PTX_STATE_SHARED";
        case PTX_STATE_TEX: return "PTX_STATE_TEX";
        default: return "UNKNOWN";
    }
}

/**
 * Debug function to print type name
 */
static const char* get_type_name(ptx_type_t type) {
    // This is a simplified version, you would need to expand it to cover all types
    switch (type) {
        case PTX_TYPE_U32: return "PTX_TYPE_U32";
        case PTX_TYPE_S32: return "PTX_TYPE_S32";
        case PTX_TYPE_F32: return "PTX_TYPE_F32";
        case PTX_TYPE_U8: return "PTX_TYPE_U8";
        case PTX_TYPE_U16: return "PTX_TYPE_U16";
        case PTX_TYPE_PRED: return "PTX_TYPE_PRED";
        case PTX_TYPE_B8: return "PTX_TYPE_B8";
        default: return "OTHER_TYPE";
    }
}

/**
 * Test basic parsing of declaration LHS
 */
void test_basic_parsing() {
    printf("Testing basic declaration LHS parsing...\n");
    
    const char* test_str = ".global .u32 loc";
    ptx_declaration_type_t lhs = {0};
    
    printf("About to parse: \"%s\"\n", test_str);
    bool result = parse_declaration_lhs(test_str, &lhs);
    printf("Parsing \"%s\": %s\n", test_str, result ? "SUCCESS" : "FAILED");
    
    if (result) {
        printf("State space: %s (%d)\n", get_state_space_name(lhs.statespace), lhs.statespace);
        printf("Type: %s (%d)\n", get_type_name(lhs.type), lhs.type);
        printf("Shape kind: %d\n", lhs.shape.kind);
        printf("Name: \"%s\"\n", lhs.name);
        
        assert(lhs.statespace == PTX_STATE_GLOBAL);
        assert(lhs.type == PTX_TYPE_U32);
        assert(lhs.shape.kind == SHAPE_SCALAR);
        assert(strcmp(lhs.name, "loc") == 0);
        
        cleanup_decl_lhs(&lhs);
        printf("Basic parsing test passed!\n\n");
    } else {
        printf("Basic parsing test failed!\n\n");
        assert(0 && "Basic parsing should not fail");
    }
}

/**
 * Test parse and print round-trip for a declaration LHS
 */
void test_parse_print_roundtrip(const char* decl_str) {
    ptx_declaration_type_t lhs = {0};
    char buffer[BUFFER_SIZE] = {0};
    
    printf("Testing roundtrip for \"%s\"\n", decl_str);
    
    // Parse the declaration string
    printf("Parsing original string...\n");
    bool parse_result = parse_declaration_lhs(decl_str, &lhs);
    if (!parse_result) {
        printf("FAILED: Could not parse declaration \"%s\"\n\n", decl_str);
        assert(parse_result && "Failed to parse declaration LHS");
        return;
    }
    
    // Debug output for parsed structure
    printf("Parsed structure:\n");
    printf("  State space: %s (%d)\n", get_state_space_name(lhs.statespace), lhs.statespace);
    printf("  Type: %s (%d)\n", get_type_name(lhs.type), lhs.type);
    printf("  Shape kind: %d\n", lhs.shape.kind);
    printf("  Name: \"%s\"\n", lhs.name);
    if (lhs.shape.kind == SHAPE_ARRAY) {
        size_t ndims = ptx_array_shape_ndims(lhs.shape.array_shape);
        printf("  Array dimensions: %zu [", ndims);
        for (size_t i = 0; i < ndims; i++) {
            printf("%zu", lhs.shape.array_shape[i]);
            if (i < ndims - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    } else if (lhs.shape.kind == SHAPE_VECTOR) {
        printf("  Vector size: %d\n", lhs.shape.vector_size);
    }
    
    // Print the parsed structure
    printf("Printing parsed structure...\n");
    int print_result = print_declaration_lhs_to_buffer(buffer, BUFFER_SIZE, &lhs);
    if (print_result <= 0) {
        printf("FAILED: Could not print declaration\n\n");
        cleanup_decl_lhs(&lhs);
        assert(print_result > 0 && "Failed to print declaration LHS");
        return;
    }
    
    // Compare the original and printed strings
    printf("Original: \"%s\"\n", decl_str);
    printf("Printed:  \"%s\"\n", buffer);
    
    // The strings may have different whitespace, so we'll parse the printed string again
    printf("Parsing printed string...\n");
    ptx_declaration_type_t lhs2 = {0};
    bool parse_result2 = parse_declaration_lhs(buffer, &lhs2);
    if (!parse_result2) {
        printf("FAILED: Could not re-parse printed declaration \"%s\"\n\n", buffer);
        cleanup_decl_lhs(&lhs);
        assert(parse_result2 && "Failed to parse printed declaration LHS");
        return;
    }
    
    // Debug output for re-parsed structure
    printf("Re-parsed structure:\n");
    printf("  State space: %s (%d)\n", get_state_space_name(lhs2.statespace), lhs2.statespace);
    printf("  Type: %s (%d)\n", get_type_name(lhs2.type), lhs2.type);
    printf("  Shape kind: %d\n", lhs2.shape.kind);
    printf("  Name: \"%s\"\n", lhs2.name);
    if (lhs2.shape.kind == SHAPE_ARRAY) {
        size_t ndims = ptx_array_shape_ndims(lhs2.shape.array_shape);
        printf("  Array dimensions: %zu [", ndims);
        for (size_t i = 0; i < ndims; i++) {
            printf("%zu", lhs2.shape.array_shape[i]);
            if (i < ndims - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    } else if (lhs2.shape.kind == SHAPE_VECTOR) {
        printf("  Vector size: %d\n", lhs2.shape.vector_size);
    }
    
    // Compare the two structures
    printf("Comparing structures...\n");
    
    if (lhs.statespace != lhs2.statespace) {
        printf("FAILED: State spaces don't match - %d vs %d\n", lhs.statespace, lhs2.statespace);
    }
    assert(lhs.statespace == lhs2.statespace && "State spaces don't match");
    
    if (lhs.type != lhs2.type) {
        printf("FAILED: Types don't match - %d vs %d\n", lhs.type, lhs2.type);
    }
    assert(lhs.type == lhs2.type && "Types don't match");
    
    if (lhs.shape.kind != lhs2.shape.kind) {
        printf("FAILED: Shape kinds don't match - %d vs %d\n", lhs.shape.kind, lhs2.shape.kind);
    }
    assert(lhs.shape.kind == lhs2.shape.kind && "Shape kinds don't match");
    
    if (lhs.shape.kind == SHAPE_VECTOR) {
        if (lhs.shape.vector_size != lhs2.shape.vector_size) {
            printf("FAILED: Vector sizes don't match - %d vs %d\n", lhs.shape.vector_size, lhs2.shape.vector_size);
        }
        assert(lhs.shape.vector_size == lhs2.shape.vector_size && "Vector sizes don't match");
    } else if (lhs.shape.kind == SHAPE_ARRAY) {
        size_t ndims1 = ptx_array_shape_ndims(lhs.shape.array_shape);
        size_t ndims2 = ptx_array_shape_ndims(lhs2.shape.array_shape);
        
        if (ndims1 != ndims2) {
            printf("FAILED: Array dimensions don't match - %zu vs %zu\n", ndims1, ndims2);
        }
        assert(ndims1 == ndims2 && "Array dimensions don't match");
        
        for (size_t i = 0; i < ndims1; i++) {
            if (lhs.shape.array_shape[i] != lhs2.shape.array_shape[i]) {
                printf("FAILED: Array sizes don't match at dimension %zu - %zu vs %zu\n", 
                       i, lhs.shape.array_shape[i], lhs2.shape.array_shape[i]);
            }
            assert(lhs.shape.array_shape[i] == lhs2.shape.array_shape[i] && "Array sizes don't match");
        }
    }
    
    if (strcmp(lhs.name, lhs2.name) != 0) {
        printf("FAILED: Names don't match - \"%s\" vs \"%s\"\n", lhs.name, lhs2.name);
    }
    assert(strcmp(lhs.name, lhs2.name) == 0 && "Names don't match");
    
    // Clean up
    cleanup_decl_lhs(&lhs);
    cleanup_decl_lhs(&lhs2);
    
    printf("Roundtrip successful!\n\n");
}

/**
 * Test invalid input handling
 */
void test_invalid_inputs() {
    printf("Testing invalid inputs...\n");
    
    ptx_declaration_type_t lhs = {0};
    
    // Test NULL input
    bool result1 = parse_declaration_lhs(NULL, &lhs);
    printf("Parse NULL string: %s\n", result1 ? "UNEXPECTED SUCCESS" : "FAILED (expected)");
    assert(!result1 && "NULL string should fail");
    
    // Test empty string
    bool result2 = parse_declaration_lhs("", &lhs);
    printf("Parse empty string: %s\n", result2 ? "UNEXPECTED SUCCESS" : "FAILED (expected)");
    assert(!result2 && "Empty string should fail");
    
    // Test invalid state space
    bool result3 = parse_declaration_lhs(".invalid .u32 x", &lhs);
    printf("Parse invalid state space: %s\n", result3 ? "UNEXPECTED SUCCESS" : "FAILED (expected)");
    assert(!result3 && "Invalid state space should fail");
    
    // Test missing type
    bool result4 = parse_declaration_lhs(".global x", &lhs);
    printf("Parse missing type: %s\n", result4 ? "UNEXPECTED SUCCESS" : "FAILED (expected)");
    assert(!result4 && "Missing type should fail");
    
    // Test invalid type
    bool result5 = parse_declaration_lhs(".global .invalid x", &lhs);
    printf("Parse invalid type: %s\n", result5 ? "UNEXPECTED SUCCESS" : "FAILED (expected)");
    assert(!result5 && "Invalid type should fail");
    
    // Test missing identifier
    bool result6 = parse_declaration_lhs(".global .u32", &lhs);
    printf("Parse missing identifier: %s\n", result6 ? "UNEXPECTED SUCCESS" : "FAILED (expected)");
    assert(!result6 && "Missing identifier should fail");
    
    printf("Invalid input tests passed!\n\n");
}

/**
 * Test complex multi-dimensional arrays
 */
void test_complex_arrays() {
    printf("Testing complex multi-dimensional arrays...\n");
    
    // Test cases for complex arrays
    const char* test_cases[] = {
        ".global .u32 matrix[2][3]",
        ".shared .f32 cube[4][4][4]",
        ".local .u8 complex[2][1][10]",
        ".const .s32 tensor[5][3][2][6]",
        ".global .f32 irregular[10][2][8][1][7]"
    };
    
    const int num_test_cases = sizeof(test_cases) / sizeof(test_cases[0]);
    
    for (int i = 0; i < num_test_cases; i++) {
        const char* test_str = test_cases[i];
        ptx_declaration_type_t lhs = {0};
        
        printf("\nTesting array: \"%s\"\n", test_str);
        bool result = parse_declaration_lhs(test_str, &lhs);
        
        assert(result && "Failed to parse complex array declaration");
        printf("Parsing result: %s\n", result ? "SUCCESS" : "FAILED");
        
        if (result) {
            printf("State space: %s (%d)\n", get_state_space_name(lhs.statespace), lhs.statespace);
            printf("Type: %s (%d)\n", get_type_name(lhs.type), lhs.type);
            assert(lhs.shape.kind == SHAPE_ARRAY);
            printf("Shape kind: %d (SHAPE_ARRAY)\n", lhs.shape.kind);
            
            size_t ndims = ptx_array_shape_ndims(lhs.shape.array_shape);
            printf("Array dimensions: %zu [", ndims);
            for (size_t j = 0; j < ndims; j++) {
                printf("%zu", lhs.shape.array_shape[j]);
                if (j < ndims - 1) {
                    printf(", ");
                }
            }
            printf("]\n");
            
            // Verify dimensions count based on the test case
            if (strstr(test_str, "[2][3]")) {
                assert(ndims == 2);
                assert(lhs.shape.array_shape[0] == 2);
                assert(lhs.shape.array_shape[1] == 3);
            } else if (strstr(test_str, "[4][4][4]")) {
                assert(ndims == 3);
                assert(lhs.shape.array_shape[0] == 4);
                assert(lhs.shape.array_shape[1] == 4);
                assert(lhs.shape.array_shape[2] == 4);
            } else if (strstr(test_str, "[2][1][10]")) {
                assert(ndims == 3);
                assert(lhs.shape.array_shape[0] == 2);
                assert(lhs.shape.array_shape[1] == 1);
                assert(lhs.shape.array_shape[2] == 10);
            } else if (strstr(test_str, "[5][3][2][6]")) {
                assert(ndims == 4);
                assert(lhs.shape.array_shape[0] == 5);
                assert(lhs.shape.array_shape[1] == 3);
                assert(lhs.shape.array_shape[2] == 2);
                assert(lhs.shape.array_shape[3] == 6);
            } else if (strstr(test_str, "[10][2][8][1][7]")) {
                assert(ndims == 5);
                assert(lhs.shape.array_shape[0] == 10);
                assert(lhs.shape.array_shape[1] == 2);
                assert(lhs.shape.array_shape[2] == 8);
                assert(lhs.shape.array_shape[3] == 1);
                assert(lhs.shape.array_shape[4] == 7);
            }
            
            cleanup_decl_lhs(&lhs);
        }
    }
    
    printf("\nComplex array tests passed!\n\n");
}

/**
 * Test parsing of declaration LHS with alignment specifier
 */
void test_alignment_parsing() {
    printf("Testing declaration LHS parsing with alignment...\n");
    
    // Test with alignment
    const char* test_str = ".const .align 4 .b8 bar[8]";
    ptx_declaration_type_t lhs = {0};
    
    printf("About to parse: \"%s\"\n", test_str);
    bool result = parse_declaration_lhs(test_str, &lhs);
    printf("Parsing \"%s\": %s\n", test_str, result ? "SUCCESS" : "FAILED");
    
    if (result) {
        printf("State space: %s (%d)\n", get_state_space_name(lhs.statespace), lhs.statespace);
        printf("Type: %s (%d)\n", get_type_name(lhs.type), lhs.type);
        printf("Shape kind: %d\n", lhs.shape.kind);
        printf("Name: \"%s\"\n", lhs.name);
        printf("Alignment: %u\n", lhs.alignment);
        
        assert(lhs.statespace == PTX_STATE_CONST);
        assert(lhs.type == PTX_TYPE_B8);
        assert(lhs.shape.kind == SHAPE_ARRAY);
        assert(strcmp(lhs.name, "bar") == 0);
        assert(lhs.alignment == 4);
        
        // Check array dimensions
        assert(lhs.shape.array_shape != NULL);
        assert(ptx_array_shape_ndims(lhs.shape.array_shape) == 1);
        assert(lhs.shape.array_shape[0] == 8);
        
        cleanup_decl_lhs(&lhs);
        printf("Alignment parsing test passed!\n\n");
    } else {
        printf("Alignment parsing test failed!\n\n");
        assert(0 && "Alignment parsing should not fail");
    }
    
    // Test with non-power-of-2 alignment (should fail)
    const char* invalid_test_str = ".const .align 3 .b8 bar[8]";
    memset(&lhs, 0, sizeof(lhs));
    
    printf("About to parse invalid alignment: \"%s\"\n", invalid_test_str);
    result = parse_declaration_lhs(invalid_test_str, &lhs);
    printf("Parsing \"%s\": %s\n", invalid_test_str, result ? "SUCCESS" : "FAILED");
    
    assert(!result && "Non-power-of-2 alignment should fail");
    printf("Invalid alignment test passed!\n\n");
}

/**
 * Test parameterized variable declarations
 */
void test_parameterized_variables() {
    printf("Testing parameterized variable declarations...\n");
    
    // Test a basic parameterized variable
    const char* test_str = ".reg .b32 %r<100>";
    ptx_declaration_type_t lhs = {0};
    
    bool result = parse_declaration_lhs(test_str, &lhs);
    assert(result && "Failed to parse parameterized variable declaration");
    
    assert(lhs.statespace == PTX_STATE_REG);
    assert(lhs.type == PTX_TYPE_B32);
    assert(lhs.shape.kind == SHAPE_SCALAR);
    assert(lhs.parameterization == 100);
    assert(strcmp(lhs.name, "%r") == 0);
    
    // Test with different state space
    const char* test_str2 = ".shared .u32 %shared<64>";
    ptx_declaration_type_t lhs2 = {0};
    
    result = parse_declaration_lhs(test_str2, &lhs2);
    assert(result && "Failed to parse parameterized shared variable declaration");
    
    assert(lhs2.statespace == PTX_STATE_SHARED);
    assert(lhs2.type == PTX_TYPE_U32);
    assert(lhs2.shape.kind == SHAPE_SCALAR);
    assert(lhs2.parameterization == 64);
    assert(strcmp(lhs2.name, "%shared") == 0);
    
    // Test with alignment
    const char* test_str3 = ".global .align 4 .f32 %g<16>";
    ptx_declaration_type_t lhs3 = {0};
    
    result = parse_declaration_lhs(test_str3, &lhs3);
    assert(result && "Failed to parse parameterized global variable with alignment");
    
    assert(lhs3.statespace == PTX_STATE_GLOBAL);
    assert(lhs3.type == PTX_TYPE_F32);
    assert(lhs3.shape.kind == SHAPE_SCALAR);
    assert(lhs3.parameterization == 16);
    assert(lhs3.alignment == 4);
    assert(strcmp(lhs3.name, "%g") == 0);
    
    // Test with vector type
    const char* test_str4 = ".reg .v2 .f32 %v<32>";
    ptx_declaration_type_t lhs4 = {0};
    
    result = parse_declaration_lhs(test_str4, &lhs4);
    assert(result && "Failed to parse parameterized vector variable");
    
    assert(lhs4.statespace == PTX_STATE_REG);
    assert(lhs4.type == PTX_TYPE_F32);
    assert(lhs4.shape.kind == SHAPE_VECTOR);
    assert(lhs4.shape.vector_size == VECTOR_SIZE_V2);
    assert(lhs4.parameterization == 32);
    assert(strcmp(lhs4.name, "%v") == 0);
    
    // Test invalid: Array variables can't use parameterization
    const char* invalid_test_str = ".reg .b32 %arr<10>[5]";
    ptx_declaration_type_t lhs5 = {0};
    
    result = parse_declaration_lhs(invalid_test_str, &lhs5);
    assert(!result && "Array variable shouldn't allow parameterization");
    
    // Clean up
    cleanup_decl_lhs(&lhs);
    cleanup_decl_lhs(&lhs2);
    cleanup_decl_lhs(&lhs3);
    cleanup_decl_lhs(&lhs4);
    
    printf("Parameterized variable declaration tests passed!\n\n");
}

int main() {
    printf("PTX Declaration LHS Parse-Print Roundtrip Tests\n");
    printf("=============================================\n\n");
    
    // Test basic parsing
    test_basic_parsing();
    
    // Test parse-print roundtrip
    test_parse_print_roundtrip(".reg .b32 %r0");
    test_parse_print_roundtrip(".global .u32 %g");
    test_parse_print_roundtrip(".const .f32 %fc");
    test_parse_print_roundtrip(".shared .b64 %s");
    test_parse_print_roundtrip(".local .u16 %l");
    test_parse_print_roundtrip(".param .s32 %p");
    
    // Test vectors
    test_parse_print_roundtrip(".reg .v2 .f32 %v2");
    test_parse_print_roundtrip(".reg .v4 .f32 %v4");
    
    // Test arrays
    test_parse_print_roundtrip(".global .u32 %arr[10]");
    test_parse_print_roundtrip(".local .f32 %multi[2][3][4]");
    
    // Test alignment
    test_parse_print_roundtrip(".global .align 4 .u32 %aligned");
    test_parse_print_roundtrip(".global .align 8 .f64 %aligned8");
    
    // Test alignment with vectors and arrays
    test_parse_print_roundtrip(".global .align 16 .v4 .f32 %aligned_vec");
    test_parse_print_roundtrip(".global .align 8 .u32 %aligned_arr[10]");
    
    // Test parameterized variables
    test_parameterized_variables();
    
    // Test parameterized variables roundtrip
    test_parse_print_roundtrip(".reg .b32 %r<100>");
    test_parse_print_roundtrip(".shared .u32 %shared<64>");
    test_parse_print_roundtrip(".global .align 4 .f32 %g<16>");
    test_parse_print_roundtrip(".reg .v2 .f32 %v<32>");
    
    // Test invalid inputs
    test_invalid_inputs();
    
    // Test complex array parsing
    test_complex_arrays();
    
    // Test alignment parsing
    test_alignment_parsing();
    
    printf("All tests passed!\n");
    return 0;
} 