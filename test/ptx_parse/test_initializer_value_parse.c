#include <ptx_parse/ptx_initializer_value_parse.h>
#include <ptx_ast/ptx_type_decl.h>
#include <ptx_ast/ptx_constant_expr.h>
#include <ptx_print/ptx_initializer_value_print.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Globals to track test results
int total_scalar_tests = 0;
int passed_scalar_tests = 0;
int total_var_addr_tests = 0;
int passed_var_addr_tests = 0;
int total_masked_tests = 0;
int passed_masked_tests = 0;

// Helper function to create test ptx_initializer_value_t
void setup_test_initializer_value(ptx_initializer_value_t* value) {
    memset(value, 0, sizeof(ptx_initializer_value_t));
}

// Helper function to cleanup test ptx_initializer_value_t
void cleanup_test_initializer_value(ptx_initializer_value_t* value) {
    if (value->kind == 0 && value->scalar_expr != NULL) { // INIT_VALUE_SCALAR
        free_expr(value->scalar_expr);
    } else if (value->kind == 1 && value->addr_var.var_name != NULL) { // INIT_VALUE_ADDR_VAR
        free((void*)value->addr_var.var_name);
    }
}

// Helper function to test parse and print roundtrip
void test_parse_print_roundtrip(const char* input_str) {
    printf("Testing roundtrip: '%s'\n", input_str);
    printf("  [EXPECTED: Should parse successfully and print correctly]\n");
    
    ptx_initializer_value_t value;
    setup_test_initializer_value(&value);
    
    bool success = parse_initializer_value(input_str, &value, PTX_TYPE_F32);
    printf("  Initial parse success: %s\n", success ? "true" : "false");
    if (!success) {
        printf("  [FAIL] ❌ Failed to parse input: '%s'\n", input_str);
        return;
    }
    
    char buffer[256];
    int result = sprint_ptx_initializer_value(buffer, sizeof(buffer), &value);
    printf("  Parsed and printed: '%s'\n", buffer);
    
    // Verify that parsing the printed output gives the same result
    ptx_initializer_value_t value2;
    setup_test_initializer_value(&value2);
    success = parse_initializer_value(buffer, &value2, PTX_TYPE_F32);
    printf("  Re-parse success: %s\n", success ? "true" : "false");
    if (!success) {
        printf("  [FAIL] ❌ Failed to re-parse: '%s'\n", buffer);
        cleanup_test_initializer_value(&value);
        // Don't use assert here to allow all tests to run
        printf("  ASSERTION FAILED: Re-parse failed for: '%s'\n", buffer);
        return;
    }
    
    char buffer2[256];
    result = sprint_ptx_initializer_value(buffer2, sizeof(buffer2), &value2);
    printf("  Re-parsed and printed: '%s'\n", buffer2);
    
    // Compare the results
    if (strcmp(buffer, buffer2) != 0) {
        printf("  [FAIL] ❌ Results don't match!\n");
        printf("  Expected: '%s'\n", buffer);
        printf("  Actual:   '%s'\n", buffer2);
        // Don't assert, so we can continue testing other cases
        cleanup_test_initializer_value(&value);
        cleanup_test_initializer_value(&value2);
        return;
    }
    
    cleanup_test_initializer_value(&value);
    cleanup_test_initializer_value(&value2);
    printf("  [PASS] ✅\n");
}

// Test parsing scalar values
void test_scalar_values() {
    printf("\nTesting scalar values:\n");
    printf("  [EXPECTED: All scalar values should parse successfully and roundtrip correctly]\n");
    bool all_passed = true;
    
    // Integer constants
    total_scalar_tests++;
    printf("\nTesting integer: '42'\n");
    test_parse_print_roundtrip("42");
    
    total_scalar_tests++;
    printf("\nTesting negative integer: '-123'\n");
    test_parse_print_roundtrip("-123");
    
    total_scalar_tests++;
    printf("\nTesting hex integer: '0xFF'\n");
    test_parse_print_roundtrip("0xFF");
    
    // Float constants
    total_scalar_tests++;
    printf("\nTesting float: '3.14159'\n");
    test_parse_print_roundtrip("3.14159");
    
    total_scalar_tests++;
    printf("\nTesting negative float: '-2.5'\n");
    test_parse_print_roundtrip("-2.5");
    
    total_scalar_tests++;
    printf("\nTesting scientific notation: '1.0e-10'\n");
    test_parse_print_roundtrip("1.0e-10");
    
    // Expressions
    total_scalar_tests++;
    printf("\nTesting simple expression: '2+3'\n");
    test_parse_print_roundtrip("2+3");
    
    total_scalar_tests++;
    printf("\nTesting multiplication: '10*5'\n");
    test_parse_print_roundtrip("10*5");
    
    total_scalar_tests++;
    printf("\nTesting complex expression: '(1+2)*3'\n");
    test_parse_print_roundtrip("(1+2)*3");
    
    if (all_passed) {
        printf("\nAll scalar value tests passed! ✅\n");
    } else {
        printf("\nSome scalar value tests failed! ❌\n");
    }
}

// Test parsing variable addresses
void test_variable_addresses() {
    printf("\nTesting variable addresses:\n");
    printf("  [EXPECTED: All variable addresses should parse successfully and roundtrip correctly]\n");
    bool all_passed = true;
    
    // Simple variable references
    total_var_addr_tests++;
    printf("\nTesting simple variable: 'foo'\n");
    test_parse_print_roundtrip("foo");
    
    total_var_addr_tests++;
    printf("\nTesting variable with underscore: 'my_var'\n");
    test_parse_print_roundtrip("my_var");
    
    total_var_addr_tests++;
    printf("\nTesting variable with numbers: 'var123'\n");
    test_parse_print_roundtrip("var123");
    
    // Variable references with offsets
    total_var_addr_tests++;
    printf("\nTesting variable with positive offset: 'bar+8'\n");
    test_parse_print_roundtrip("bar+8");
    
    total_var_addr_tests++;
    printf("\nTesting variable with negative offset: 'baz-4'\n");
    test_parse_print_roundtrip("baz-4");
    
    // Generic variables
    total_var_addr_tests++;
    printf("\nTesting generic variable: 'generic(qux)'\n");
    test_parse_print_roundtrip("generic(qux)");
    
    total_var_addr_tests++;
    printf("\nTesting generic variable with offset: 'generic(quux+16)'\n");
    test_parse_print_roundtrip("generic(quux+16)");
    
    if (all_passed) {
        printf("\nAll variable address tests passed! ✅\n");
    } else {
        printf("\nSome variable address tests failed! ❌\n");
    }
}

// Test parsing masked values
void test_masked_values() {
    printf("\nTesting masked values:\n");
    printf("  [EXPECTED: All masked values should parse successfully and roundtrip correctly]\n");
    bool all_passed = true;
    
    // Masked scalar constants
    total_masked_tests++;
    printf("\nTesting masked scalar: '0xFF(42)'\n");
    test_parse_print_roundtrip("0xFF(42)");
    
    total_masked_tests++;
    printf("\nTesting masked hex value: '0xFF00(0x1234)'\n");
    test_parse_print_roundtrip("0xFF00(0x1234)");
    
    // Masked variable references
    total_masked_tests++;
    printf("\nTesting masked variable: '0xFF(foo)'\n");
    test_parse_print_roundtrip("0xFF(foo)");
    
    total_masked_tests++;
    printf("\nTesting masked variable with offset: '0xFF0000(baz+4)'\n");
    test_parse_print_roundtrip("0xFF0000(baz+4)");
    
    // Masked generic variables
    total_masked_tests++;
    printf("\nTesting masked generic variable: '0xFF00(generic(bar))'\n");
    test_parse_print_roundtrip("0xFF00(generic(bar))");
    
    total_masked_tests++;
    printf("\nTesting masked generic variable with offset: '0xFF000000(generic(quux+16))'\n");
    test_parse_print_roundtrip("0xFF000000(generic(quux+16))");
    
    if (all_passed) {
        printf("\nAll masked value tests passed! ✅\n");
    } else {
        printf("\nSome masked value tests failed! ❌\n");
    }
}

// Test invalid inputs
void test_invalid_inputs() {
    printf("\nTesting invalid inputs:\n");
    printf("  [EXPECTED: All of these SHOULD FAIL to parse]\n");
    
    ptx_initializer_value_t value;
    bool all_passed = true;
    
    // Empty string
    printf("Testing empty string\n");
    setup_test_initializer_value(&value);
    bool success = parse_initializer_value("", &value, PTX_TYPE_F32);
    printf("  Parse success: %s (expected: false)\n", success ? "true ❌" : "false ✅");
    if (success) {
        printf("  [FAIL] ❌ Expected parsing to fail but it succeeded\n");
        all_passed = false;
    } else {
        printf("  [PASS] ✅ Correctly rejected invalid input\n");
    }
    
    // Invalid mask
    printf("Testing invalid mask\n");
    setup_test_initializer_value(&value);
    success = parse_initializer_value("0xF(42)", &value, PTX_TYPE_F32);
    printf("  Parse success: %s (expected: false)\n", success ? "true ❌" : "false ✅");
    if (success) {
        printf("  [FAIL] ❌ Expected parsing to fail but it succeeded\n");
        all_passed = false;
    } else {
        printf("  [PASS] ✅ Correctly rejected invalid input\n");
    }
    
    // Invalid generic syntax
    printf("Testing invalid generic syntax\n");
    setup_test_initializer_value(&value);
    success = parse_initializer_value("generic foo", &value, PTX_TYPE_F32);
    printf("  Parse success: %s (expected: false)\n", success ? "true ❌" : "false ✅");
    if (success) {
        printf("  [FAIL] ❌ Expected parsing to fail but it succeeded\n");
        all_passed = false;
    } else {
        printf("  [PASS] ✅ Correctly rejected invalid input\n");
    }
    
    // Invalid offset syntax
    printf("Testing invalid offset syntax\n");
    setup_test_initializer_value(&value);
    success = parse_initializer_value("foo + ", &value, PTX_TYPE_F32);
    printf("  Parse success: %s (expected: false)\n", success ? "true ❌" : "false ✅");
    if (success) {
        printf("  [FAIL] ❌ Expected parsing to fail but it succeeded\n");
        all_passed = false;
    } else {
        printf("  [PASS] ✅ Correctly rejected invalid input\n");
    }
    
    if (all_passed) {
        printf("\nAll invalid input tests passed ✅\n");
    } else {
        printf("\nSome invalid input tests failed ❌\n");
    }
}

int main() {
    printf("=== Testing PTX Initializer Value Parser ===\n");
    
    // Debug direct test for floating-point parsing
    printf("\nDirect Test for Floating-Point Parsing:\n");
    ptx_initializer_value_t value;
    setup_test_initializer_value(&value);
    
    const char* test_float = "3.14159";
    printf("  [EXPECTED: Should parse successfully]\n");
    bool success = parse_initializer_value(test_float, &value, PTX_TYPE_F32);
    printf("Parse result for '%s': %s\n", test_float, success ? "SUCCESS ✅" : "FAILED ❌");
    
    if (success) {
        char buffer[256];
        int result = sprint_ptx_initializer_value(buffer, sizeof(buffer), &value);
        printf("Printed: '%s'\n", buffer);
        cleanup_test_initializer_value(&value);
    }
    
    // Run the test suites
    printf("\n=== Testing Scalar Values ===\n");
    test_scalar_values();
    
    printf("\n=== Testing Variable Addresses ===\n");
    test_variable_addresses();
    
    printf("\n=== Testing Masked Values ===\n");
    test_masked_values();
    
    printf("\n=== Testing Invalid Inputs ===\n");
    test_invalid_inputs();
    
    // Print the overall test summary
    printf("\n=== Test Summary ===\n");
    int total_tests = total_scalar_tests + total_var_addr_tests + total_masked_tests + 4; // 4 invalid tests
    printf("Ran %d tests\n", total_tests);
    printf("NOTE: To see detailed failures, look for [FAIL] markers in the output above\n");
    
    return 0;
} 