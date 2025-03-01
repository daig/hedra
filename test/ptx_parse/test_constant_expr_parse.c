#include <ptx_parse/ptx_constant_expr_parse.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

// Helper to check if an expression is of constant type with a specific value
bool is_constant_int(ptx_expr_t* expr, int64_t expected_value) {
    if (!expr || expr->type != EXPR_CONSTANT) {
        return false;
    }
    return expr->constant.type == PTX_CONST_INT_SIGNED && 
           expr->constant.s64_val == expected_value;
}

// Helper to check if an expression is of unary type with a specific operator and value
bool is_unary_op(ptx_expr_t* expr, operator_t expected_op, int64_t expected_value) {
    if (!expr || expr->type != EXPR_UNARY_OP) {
        return false;
    }
    return expr->unary.op == expected_op && 
           is_constant_int(expr->unary.operand, expected_value);
}

// Helper to check if an expression is of binary type with a specific operator and values
bool is_binary_op(ptx_expr_t* expr, operator_t expected_op, int64_t expected_left, int64_t expected_right) {
    if (!expr || expr->type != EXPR_BINARY_OP) {
        return false;
    }
    return expr->binary.op == expected_op && 
           is_constant_int(expr->binary.left, expected_left) && 
           is_constant_int(expr->binary.right, expected_right);
}

// Helper to check if an expression is of ternary type with specific values
bool is_ternary_op(ptx_expr_t* expr, int64_t cond_val, int64_t true_val, int64_t false_val) {
    if (!expr || expr->type != EXPR_TERNARY_OP) {
        return false;
    }
    return is_constant_int(expr->ternary.condition, cond_val) && 
           is_constant_int(expr->ternary.true_expr, true_val) && 
           is_constant_int(expr->ternary.false_expr, false_val);
}

void test_primary_expr() {
    printf("Testing primary expression parsing...\n");
    
    ptx_expr_t* expr = NULL;
    int pos = 0;
    
    // Test integer constant
    const char* int_str = "42";
    assert(parse_primary_expr(int_str, &pos, &expr));
    assert(is_constant_int(expr, 42));
    free_expr(expr);
    
    // Test parenthesized expression
    pos = 0;
    const char* paren_str = "(42)";
    assert(parse_primary_expr(paren_str, &pos, &expr));
    assert(is_constant_int(expr, 42));
    free_expr(expr);
    
    printf("Primary expression parsing tests passed!\n");
}

void test_unary_expr() {
    printf("Testing unary expression parsing...\n");
    
    ptx_expr_t* expr = NULL;
    int pos = 0;
    
    // Test positive unary
    const char* plus_str = "+42";
    assert(parse_unary_expr(plus_str, &pos, &expr));
    assert(is_unary_op(expr, PLUS, 42));
    free_expr(expr);
    
    // Test negative unary
    pos = 0;
    const char* minus_str = "-42";
    assert(parse_unary_expr(minus_str, &pos, &expr));
    assert(is_unary_op(expr, MINUS, 42));
    free_expr(expr);
    
    // Test logical not unary
    pos = 0;
    const char* not_str = "!0";
    assert(parse_unary_expr(not_str, &pos, &expr));
    assert(is_unary_op(expr, NOT, 0));
    free_expr(expr);
    
    // Test bitwise complement unary
    pos = 0;
    const char* complement_str = "~42";
    assert(parse_unary_expr(complement_str, &pos, &expr));
    assert(is_unary_op(expr, COMPLEMENT, 42));
    free_expr(expr);
    
    printf("Unary expression parsing tests passed!\n");
}

void test_binary_expr() {
    printf("Testing binary expression parsing...\n");
    
    ptx_expr_t* expr = NULL;
    
    // Test addition
    const char* add_str = "5 + 3";
    assert(parse_expr(add_str, &expr));
    assert(is_binary_op(expr, ADD, 5, 3));
    free_expr(expr);
    
    // Test subtraction
    const char* sub_str = "10 - 4";
    assert(parse_expr(sub_str, &expr));
    assert(is_binary_op(expr, SUB, 10, 4));
    free_expr(expr);
    
    // Test multiplication
    const char* mul_str = "6 * 7";
    assert(parse_expr(mul_str, &expr));
    assert(is_binary_op(expr, MUL, 6, 7));
    free_expr(expr);
    
    // Test division
    const char* div_str = "20 / 5";
    assert(parse_expr(div_str, &expr));
    assert(is_binary_op(expr, DIV, 20, 5));
    free_expr(expr);
    
    // Test modulus
    const char* mod_str = "17 % 5";
    assert(parse_expr(mod_str, &expr));
    assert(is_binary_op(expr, MOD, 17, 5));
    free_expr(expr);
    
    // Test bitwise AND
    const char* and_str = "12 & 5";
    assert(parse_expr(and_str, &expr));
    assert(is_binary_op(expr, AND, 12, 5));
    free_expr(expr);
    
    // Test bitwise OR
    const char* or_str = "12 | 5";
    assert(parse_expr(or_str, &expr));
    assert(is_binary_op(expr, OR, 12, 5));
    free_expr(expr);
    
    // Test bitwise XOR
    const char* xor_str = "12 ^ 5";
    assert(parse_expr(xor_str, &expr));
    assert(is_binary_op(expr, XOR, 12, 5));
    free_expr(expr);
    
    // Test shift left
    const char* shl_str = "1 << 3";
    assert(parse_expr(shl_str, &expr));
    assert(is_binary_op(expr, SHL, 1, 3));
    free_expr(expr);
    
    // Test shift right
    const char* shr_str = "16 >> 2";
    assert(parse_expr(shr_str, &expr));
    assert(is_binary_op(expr, SHR, 16, 2));
    free_expr(expr);
    
    printf("Binary expression parsing tests passed!\n");
}

void test_comparison_expr() {
    printf("Testing comparison expression parsing...\n");
    
    ptx_expr_t* expr = NULL;
    
    // Test equal to
    const char* eq_str = "5 == 5";
    assert(parse_expr(eq_str, &expr));
    assert(is_binary_op(expr, EQ, 5, 5));
    free_expr(expr);
    
    // Test not equal to
    const char* ne_str = "5 != 3";
    assert(parse_expr(ne_str, &expr));
    assert(is_binary_op(expr, NE, 5, 3));
    free_expr(expr);
    
    // Test less than
    const char* lt_str = "3 < 5";
    assert(parse_expr(lt_str, &expr));
    assert(is_binary_op(expr, LT, 3, 5));
    free_expr(expr);
    
    // Test less than or equal to
    const char* le_str = "3 <= 5";
    assert(parse_expr(le_str, &expr));
    assert(is_binary_op(expr, LE, 3, 5));
    free_expr(expr);
    
    // Test greater than
    const char* gt_str = "5 > 3";
    assert(parse_expr(gt_str, &expr));
    assert(is_binary_op(expr, GT, 5, 3));
    free_expr(expr);
    
    // Test greater than or equal to
    const char* ge_str = "5 >= 3";
    assert(parse_expr(ge_str, &expr));
    assert(is_binary_op(expr, GE, 5, 3));
    free_expr(expr);
    
    printf("Comparison expression parsing tests passed!\n");
}

void test_logical_expr() {
    printf("Testing logical expression parsing...\n");
    
    ptx_expr_t* expr = NULL;
    
    // Test logical AND
    const char* and_logical_str = "1 && 1";
    assert(parse_expr(and_logical_str, &expr));
    assert(is_binary_op(expr, AND_LOGICAL, 1, 1));
    free_expr(expr);
    
    // Test logical OR
    const char* or_logical_str = "1 || 0";
    assert(parse_expr(or_logical_str, &expr));
    assert(is_binary_op(expr, OR_LOGICAL, 1, 0));
    free_expr(expr);
    
    printf("Logical expression parsing tests passed!\n");
}

void test_ternary_expr() {
    printf("Testing ternary expression parsing...\n");
    
    ptx_expr_t* expr = NULL;
    
    // Test ternary operator
    const char* ternary_str = "1 ? 42 : 24";
    assert(parse_expr(ternary_str, &expr));
    assert(is_ternary_op(expr, 1, 42, 24));
    free_expr(expr);
    
    printf("Ternary expression parsing tests passed!\n");
}

void test_precedence() {
    printf("Testing operator precedence...\n");
    
    ptx_expr_t* expr = NULL;
    
    // Test 1 + 2 * 3 (multiplication has higher precedence)
    const char* precedence_str = "1 + 2 * 3";
    assert(parse_expr(precedence_str, &expr));
    assert(expr->type == EXPR_BINARY_OP);
    assert(expr->binary.op == ADD);
    assert(is_constant_int(expr->binary.left, 1));
    assert(expr->binary.right->type == EXPR_BINARY_OP);
    assert(expr->binary.right->binary.op == MUL);
    assert(is_constant_int(expr->binary.right->binary.left, 2));
    assert(is_constant_int(expr->binary.right->binary.right, 3));
    free_expr(expr);
    
    // Test (1 + 2) * 3 (parentheses override precedence)
    const char* paren_precedence_str = "(1 + 2) * 3";
    assert(parse_expr(paren_precedence_str, &expr));
    assert(expr->type == EXPR_BINARY_OP);
    assert(expr->binary.op == MUL);
    assert(expr->binary.left->type == EXPR_BINARY_OP);
    assert(expr->binary.left->binary.op == ADD);
    assert(is_constant_int(expr->binary.left->binary.left, 1));
    assert(is_constant_int(expr->binary.left->binary.right, 2));
    assert(is_constant_int(expr->binary.right, 3));
    free_expr(expr);
    
    printf("Operator precedence tests passed!\n");
}

void test_complex_expr() {
    printf("Testing complex expressions...\n");
    
    ptx_expr_t* expr = NULL;
    
    // Test complex expression with multiple operators
    const char* complex_str = "5 * (3 + 2) - 4 / 2";
    assert(parse_expr(complex_str, &expr));
    // We don't check the exact structure, just that it parses successfully
    free_expr(expr);
    
    // Test ternary with logical operators
    const char* ternary_complex_str = "1 && 1 ? 10 + 5 : 20 - 5";
    assert(parse_expr(ternary_complex_str, &expr));
    // We don't check the exact structure, just that it parses successfully
    free_expr(expr);
    
    printf("Complex expression tests passed!\n");
}

void test_invalid_expr() {
    printf("Testing invalid expressions...\n");
    
    ptx_expr_t* expr = NULL;
    
    // Test empty string
    const char* empty_str = "";
    assert(parse_expr(empty_str, &expr) == false);
    
    // Test mismatched parentheses
    const char* mismatched_paren_str = "(1 + 2";
    assert(parse_expr(mismatched_paren_str, &expr) == false);
    
    // Test invalid token
    const char* invalid_token_str = "1 + @";
    assert(parse_expr(invalid_token_str, &expr) == false);
    
    // Test invalid ternary (missing colon)
    const char* invalid_ternary_str = "1 ? 2";
    assert(parse_expr(invalid_ternary_str, &expr) == false);
    
    printf("Invalid expression tests passed!\n");
}

int main() {
    printf("Running PTX expression parsing tests...\n");
    
    test_primary_expr();
    test_unary_expr();
    test_binary_expr();
    test_comparison_expr();
    test_logical_expr();
    test_ternary_expr();
    test_precedence();
    test_complex_expr();
    test_invalid_expr();
    
    printf("All expression parsing tests passed!\n");
    return 0;
} 