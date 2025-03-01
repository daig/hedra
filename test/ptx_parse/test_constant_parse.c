#include <ptx_parse/ptx_constant_parse.h>
#include <stdio.h>
#include <assert.h>

void test_int_parsing() {
    printf("Testing integer parsing...\n");
    
    ptx_constant_t constant;
    
    // Test decimal
    assert(parse_int_literal("123", &constant) == true);
    assert(constant.type == PTX_CONST_INT_SIGNED);
    assert(constant.s64_val == 123);
    
    // Test hexadecimal
    assert(parse_int_literal("0xFF", &constant) == true);
    assert(constant.type == PTX_CONST_INT_SIGNED);
    assert(constant.s64_val == 255);
    
    // Test unsigned
    assert(parse_int_literal("42U", &constant) == true);
    assert(constant.type == PTX_CONST_INT_UNSIGNED);
    assert(constant.u64_val == 42);
    
    printf("Integer parsing tests passed!\n");
}

void test_float_parsing() {
    printf("Testing float parsing...\n");
    
    ptx_constant_t constant;
    
    // Test decimal float
    assert(parse_float_literal("123.456", &constant) == true);
    assert(constant.type == PTX_CONST_FLOAT);
    assert(constant.f64_val > 123.455 && constant.f64_val < 123.457);
    
    printf("Float parsing tests passed!\n");
}

void test_pred_parsing() {
    printf("Testing predicate parsing...\n");
    
    ptx_constant_t constant;
    
    // Test predicate 0 (false)
    assert(parse_pred_literal("0", &constant) == true);
    assert(constant.type == PTX_CONST_PRED);
    assert(constant.pred_val == 0);
    
    // Test predicate 1 (true)
    assert(parse_pred_literal("1", &constant) == true);
    assert(constant.type == PTX_CONST_PRED);
    assert(constant.pred_val == 1);
    
    printf("Predicate parsing tests passed!\n");
}

void test_constant_parsing() {
    printf("Testing general constant parsing...\n");
    
    ptx_constant_t* constant = NULL;
    
    // Test integer constant
    assert(parse_constant("42", &constant) == true);
    assert(constant->type == PTX_CONST_INT_SIGNED);
    assert(constant->s64_val == 42);
    free(constant);
    
    // Test hex integer constant
    assert(parse_constant("0xABC", &constant) == true);
    assert(constant->type == PTX_CONST_INT_SIGNED);
    assert(constant->s64_val == 0xABC);
    free(constant);
    
    // Test unsigned integer constant
    assert(parse_constant("123U", &constant) == true);
    assert(constant->type == PTX_CONST_INT_UNSIGNED);
    assert(constant->u64_val == 123);
    free(constant);
    
    // Test float constant
    assert(parse_constant("3.14159", &constant) == true);
    assert(constant->type == PTX_CONST_FLOAT);
    assert(constant->f64_val > 3.14158 && constant->f64_val < 3.14160);
    free(constant);
    
    // Test with whitespace
    assert(parse_constant("  42  ", &constant) == true);
    assert(constant->type == PTX_CONST_INT_SIGNED);
    assert(constant->s64_val == 42);
    free(constant);
    
    // Test invalid constant
    assert(parse_constant("not_a_constant", &constant) == false);
    assert(constant == NULL);
    
    // Test NULL input
    assert(parse_constant(NULL, &constant) == false);
    
    printf("General constant parsing tests passed!\n");
}

int main() {
    printf("Running PTX constant parsing tests...\n");
    
    test_int_parsing();
    test_float_parsing();
    test_pred_parsing();
    test_constant_parsing();
    
    printf("All parsing tests passed!\n");
    return 0;
} 