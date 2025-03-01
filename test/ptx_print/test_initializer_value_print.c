#include <ptx_print/ptx_initializer_value_print.h>
#include <ptx_ast/ptx_constant_expr.h>
#include <ptx_ast/ptx_type_decl.h>
#include <ptx_ast/ptx_constant.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

// Helper function to create an integer expression
ptx_expr_t* ptx_expr_create_int(int value) {
    ptx_constant_t constant;
    constant.type = PTX_CONST_INT_SIGNED;
    constant.s64_val = value;
    return create_constant_expr(constant);
}

// Helper function to free an expression
void ptx_expr_free(ptx_expr_t* expr) {
    free_expr(expr);
}

// Helper function to create a scalar initializer value
ptx_initializer_value_t create_scalar_value(int value, ptx_mask_value_t mask) {
    ptx_initializer_value_t init_value;
    init_value.kind = 0; // INIT_VALUE_SCALAR
    init_value.mask = mask;
    init_value.scalar_expr = ptx_expr_create_int(value);
    return init_value;
}

// Helper function to clean up a scalar initializer value
void free_scalar_value(ptx_initializer_value_t* value) {
    if (value->kind == 0 && value->scalar_expr != NULL) { // INIT_VALUE_SCALAR
        ptx_expr_free(value->scalar_expr);
    }
}

// Helper function to create a variable address initializer value
ptx_initializer_value_t create_var_addr_value(const char* var_name, int64_t offset, bool is_generic, ptx_mask_value_t mask) {
    ptx_initializer_value_t init_value;
    init_value.kind = 1; // INIT_VALUE_ADDR_VAR
    init_value.mask = mask;
    init_value.addr_var.var_name = strdup(var_name);
    init_value.addr_var.offset = offset;
    init_value.addr_var.is_generic = is_generic;
    return init_value;
}

// Helper function to clean up a variable address initializer value
void free_var_addr_value(ptx_initializer_value_t* value) {
    if (value->kind == 1 && value->addr_var.var_name != NULL) { // INIT_VALUE_ADDR_VAR
        free((void*)value->addr_var.var_name);
    }
}

void test_scalar_printing() {
    printf("Testing scalar initializer value printing...\n");
    
    // Simple integer
    ptx_initializer_value_t value = create_scalar_value(42, MASK_UNMASKED);
    
    char buffer[256];
    int result = sprint_ptx_initializer_value(buffer, sizeof(buffer), &value);
    printf("result = %d, buffer = '%s'\n", result, buffer);
    assert(result > 0);
    assert(strcmp(buffer, "42") == 0);
    printf("Basic scalar print test passed: '%s'\n", buffer);
    
    // Integer with mask
    free_scalar_value(&value);
    value = create_scalar_value(42, MASK_BYTE0);
    
    result = sprint_ptx_initializer_value(buffer, sizeof(buffer), &value);
    assert(result > 0);
    assert(strcmp(buffer, "0xFF(42)") == 0);
    printf("Masked scalar print test passed: '%s'\n", buffer);
    
    free_scalar_value(&value);
    printf("PASS: Scalar initializer value printing\n\n");
}

void test_var_addr_printing() {
    printf("Testing variable address initializer value printing...\n");
    
    // Simple variable reference
    ptx_initializer_value_t value = create_var_addr_value("foo", 0, false, MASK_UNMASKED);
    
    char buffer[256];
    int result = sprint_ptx_initializer_value(buffer, sizeof(buffer), &value);
    assert(result > 0);
    assert(strcmp(buffer, "foo") == 0);
    printf("Basic variable reference test passed: '%s'\n", buffer);
    
    // Variable with offset
    free_var_addr_value(&value);
    value = create_var_addr_value("bar", 8, false, MASK_UNMASKED);
    
    result = sprint_ptx_initializer_value(buffer, sizeof(buffer), &value);
    assert(result > 0);
    assert(strcmp(buffer, "bar+8") == 0);
    printf("Variable with offset test passed: '%s'\n", buffer);
    
    // Variable with negative offset
    free_var_addr_value(&value);
    value = create_var_addr_value("baz", -4, false, MASK_UNMASKED);
    
    result = sprint_ptx_initializer_value(buffer, sizeof(buffer), &value);
    assert(result > 0);
    assert(strcmp(buffer, "baz-4") == 0);
    printf("Variable with negative offset test passed: '%s'\n", buffer);
    
    // Generic variable reference
    free_var_addr_value(&value);
    value = create_var_addr_value("qux", 0, true, MASK_UNMASKED);
    
    result = sprint_ptx_initializer_value(buffer, sizeof(buffer), &value);
    assert(result > 0);
    assert(strcmp(buffer, "generic(qux)") == 0);
    printf("Generic variable reference test passed: '%s'\n", buffer);
    
    // Generic variable with offset
    free_var_addr_value(&value);
    value = create_var_addr_value("quux", 16, true, MASK_UNMASKED);
    
    result = sprint_ptx_initializer_value(buffer, sizeof(buffer), &value);
    assert(result > 0);
    assert(strcmp(buffer, "generic(quux+16)") == 0);
    printf("Generic variable with offset test passed: '%s'\n", buffer);
    
    free_var_addr_value(&value);
    printf("PASS: Variable address initializer value printing\n\n");
}

void test_masked_var_addr_printing() {
    printf("Testing masked variable address initializer value printing...\n");
    
    // Variable with mask
    ptx_initializer_value_t value = create_var_addr_value("foo", 0, false, MASK_BYTE0);
    
    char buffer[256];
    int result = sprint_ptx_initializer_value(buffer, sizeof(buffer), &value);
    assert(result > 0);
    assert(strcmp(buffer, "0xFF(foo)") == 0);
    printf("Masked variable reference test passed: '%s'\n", buffer);
    
    // Generic variable with mask
    free_var_addr_value(&value);
    value = create_var_addr_value("bar", 0, true, MASK_BYTE1);
    
    result = sprint_ptx_initializer_value(buffer, sizeof(buffer), &value);
    assert(result > 0);
    assert(strcmp(buffer, "0xFF00(generic(bar))") == 0);
    printf("Masked generic variable reference test passed: '%s'\n", buffer);
    
    // Variable with offset and mask
    free_var_addr_value(&value);
    value = create_var_addr_value("baz", 4, false, MASK_BYTE2);
    
    result = sprint_ptx_initializer_value(buffer, sizeof(buffer), &value);
    assert(result > 0);
    assert(strcmp(buffer, "0xFF0000(baz+4)") == 0);
    printf("Masked variable with offset test passed: '%s'\n", buffer);
    
    free_var_addr_value(&value);
    printf("PASS: Masked variable address initializer value printing\n\n");
}

int main() {
    printf("=== Testing PTX Initializer Value Printer ===\n\n");
    
    test_scalar_printing();
    test_var_addr_printing();
    test_masked_var_addr_printing();
    
    printf("All tests passed!\n");
    return 0;
} 