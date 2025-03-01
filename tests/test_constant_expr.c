#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ptx_ast/>ptx_constant_expr.h>
#include <ptx_ast/>ptx_constant.h>

// Utility function to create signed integer constant
ptx_expr_t* create_int_constant(int64_t value) {
    ptx_constant_t constant;
    constant.type = PTX_CONST_INT_SIGNED;
    constant.s64_val = value;
    return create_constant_expr(constant);
}

// Utility function to create unsigned integer constant
ptx_expr_t* create_uint_constant(uint64_t value) {
    ptx_constant_t constant;
    constant.type = PTX_CONST_INT_UNSIGNED;
    constant.u64_val = value;
    return create_constant_expr(constant);
}

// Utility function to create floating-point constant
ptx_expr_t* create_float_constant(double value) {
    ptx_constant_t constant;
    constant.type = PTX_CONST_FLOAT;
    constant.f64_val = value;
    return create_constant_expr(constant);
}

// Utility function to create single-precision floating-point constant
ptx_expr_t* create_float_single_constant(float value) {
    ptx_constant_t constant;
    constant.type = PTX_CONST_FLOAT_SINGLE;
    constant.f32_val = value;
    return create_constant_expr(constant);
}

// Function to print the result of evaluating an expression
void print_expr_result(const char* expr_str, ptx_expr_t* expr) {
    ptx_constant_t result;
    bool success = evaluate_expr(expr, &result);
    
    printf("Expression: %s\n", expr_str);
    
    if (success) {
        switch (result.type) {
            case PTX_CONST_INT_SIGNED:
                printf("Result: %lld (signed integer)\n", (long long)result.s64_val);
                break;
            case PTX_CONST_INT_UNSIGNED:
                printf("Result: %llu (unsigned integer)\n", (unsigned long long)result.u64_val);
                break;
            case PTX_CONST_FLOAT:
                printf("Result: %f (double-precision float)\n", result.f64_val);
                break;
            case PTX_CONST_FLOAT_SINGLE:
                printf("Result: %f (single-precision float)\n", (double)result.f32_val);
                break;
            case PTX_CONST_PRED:
                printf("Result: %s (predicate)\n", result.pred_val ? "true" : "false");
                break;
            default:
                printf("Result: Unknown type\n");
        }
    } else {
        printf("Evaluation failed\n");
    }
    printf("\n");
}

int main() {
    // Array of expression strings for reference
    const char* ptx_constant_expressions[] = {
        "42",                    // Decimal integer
        "0x2A",                  // Hexadecimal integer (42 in decimal)
        "3.14",                  // Floating-point number
        "1.23e2",                // Floating-point number in scientific notation (123.0)
        "-5",                    // Negation of an integer
        "~0xFF",                 // Bitwise NOT of a hexadecimal integer
        "10 + 20",               // Addition
        "30 - 15",               // Subtraction
        "5 * 6",                 // Multiplication
        "100 / 4",               // Division
        "10 % 3",                // Modulus
        "0xFF & 0x0F",           // Bitwise AND
        "0xF0 | 0x0F",           // Bitwise OR
        "0xAA ^ 0x55",           // Bitwise XOR
        "(2 + 3) * 4",           // Grouping with addition and multiplication
        "((1 << 2) + 3) * (4 - 2)", // Nested parentheses with shifts and arithmetic
        "(1 + 2) * (3 - 4) / 5", // Combination of addition, subtraction, multiplication, and division
        "(0xF << 4) | 0xA",      // Left shift and bitwise OR
        "-10 + 20",              // Addition with a negative literal
        "(10 + 3) % 7",          // Addition and modulus
        "2 * 3 + 4 * 5",         // Testing operator precedence (multiplication before addition)
        "(1 << 5) & 0x1F",       // Left shift and bitwise AND
        "1.5 + 2.5",             // Floating-point addition
        "1.23e2",                // Floating-point literal in scientific notation (repeated for clarity)
        "1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10", // A sequence of additions
        "((1 + 2) * (3 - 4)) / (5 % 2) + (6 << 1)" // Complex expression with multiple operations
    };
    
    // Create expression trees for each expression
    
    // 1. Decimal integer: 42
    ptx_expr_t* expr1 = create_int_constant(42);
    print_expr_result(ptx_constant_expressions[0], expr1);
    
    // 2. Hexadecimal integer: 0x2A (42 in decimal)
    ptx_expr_t* expr2 = create_uint_constant(0x2A);
    print_expr_result(ptx_constant_expressions[1], expr2);
    
    // 3. Floating-point number: 3.14
    ptx_expr_t* expr3 = create_float_constant(3.14);
    print_expr_result(ptx_constant_expressions[2], expr3);
    
    // 4. Floating-point number in scientific notation: 1.23e2 (123.0)
    ptx_expr_t* expr4 = create_float_constant(1.23e2);
    print_expr_result(ptx_constant_expressions[3], expr4);
    
    // 5. Negation of an integer: -5
    ptx_expr_t* expr5 = create_unary_expr(MINUS, create_int_constant(5));
    print_expr_result(ptx_constant_expressions[4], expr5);
    
    // 6. Bitwise NOT of a hexadecimal integer: ~0xFF
    ptx_expr_t* expr6 = create_unary_expr(COMPLEMENT, create_uint_constant(0xFF));
    print_expr_result(ptx_constant_expressions[5], expr6);
    
    // 7. Addition: 10 + 20
    ptx_expr_t* expr7 = create_binary_expr(ADD, 
                        create_int_constant(10), 
                        create_int_constant(20));
    print_expr_result(ptx_constant_expressions[6], expr7);
    
    // 8. Subtraction: 30 - 15
    ptx_expr_t* expr8 = create_binary_expr(SUB, 
                        create_int_constant(30), 
                        create_int_constant(15));
    print_expr_result(ptx_constant_expressions[7], expr8);
    
    // 9. Multiplication: 5 * 6
    ptx_expr_t* expr9 = create_binary_expr(MUL, 
                        create_int_constant(5), 
                        create_int_constant(6));
    print_expr_result(ptx_constant_expressions[8], expr9);
    
    // 10. Division: 100 / 4
    ptx_expr_t* expr10 = create_binary_expr(DIV, 
                         create_int_constant(100), 
                         create_int_constant(4));
    print_expr_result(ptx_constant_expressions[9], expr10);
    
    // 11. Modulus: 10 % 3
    ptx_expr_t* expr11 = create_binary_expr(MOD, 
                         create_int_constant(10), 
                         create_int_constant(3));
    print_expr_result(ptx_constant_expressions[10], expr11);
    
    // 12. Bitwise AND: 0xFF & 0x0F
    ptx_expr_t* expr12 = create_binary_expr(AND, 
                         create_uint_constant(0xFF), 
                         create_uint_constant(0x0F));
    print_expr_result(ptx_constant_expressions[11], expr12);
    
    // 13. Bitwise OR: 0xF0 | 0x0F
    ptx_expr_t* expr13 = create_binary_expr(OR, 
                         create_uint_constant(0xF0), 
                         create_uint_constant(0x0F));
    print_expr_result(ptx_constant_expressions[12], expr13);
    
    // 14. Bitwise XOR: 0xAA ^ 0x55
    ptx_expr_t* expr14 = create_binary_expr(XOR, 
                         create_uint_constant(0xAA), 
                         create_uint_constant(0x55));
    print_expr_result(ptx_constant_expressions[13], expr14);
    
    // 15. Grouping with addition and multiplication: (2 + 3) * 4
    ptx_expr_t* expr15 = create_binary_expr(MUL, 
                         create_binary_expr(ADD, 
                                          create_int_constant(2), 
                                          create_int_constant(3)), 
                         create_int_constant(4));
    print_expr_result(ptx_constant_expressions[14], expr15);
    
    // 16. Nested parentheses with shifts and arithmetic: ((1 << 2) + 3) * (4 - 2)
    ptx_expr_t* expr16 = create_binary_expr(MUL, 
                         create_binary_expr(ADD, 
                                          create_binary_expr(SHL, 
                                                           create_int_constant(1), 
                                                           create_int_constant(2)), 
                                          create_int_constant(3)), 
                         create_binary_expr(SUB, 
                                          create_int_constant(4), 
                                          create_int_constant(2)));
    print_expr_result(ptx_constant_expressions[15], expr16);
    
    // 17. Combination of addition, subtraction, multiplication, and division: (1 + 2) * (3 - 4) / 5
    ptx_expr_t* expr17 = create_binary_expr(DIV, 
                         create_binary_expr(MUL, 
                                          create_binary_expr(ADD, 
                                                           create_int_constant(1), 
                                                           create_int_constant(2)), 
                                          create_binary_expr(SUB, 
                                                           create_int_constant(3), 
                                                           create_int_constant(4))), 
                         create_int_constant(5));
    print_expr_result(ptx_constant_expressions[16], expr17);
    
    // 18. Left shift and bitwise OR: (0xF << 4) | 0xA
    ptx_expr_t* expr18 = create_binary_expr(OR, 
                         create_binary_expr(SHL, 
                                          create_uint_constant(0xF), 
                                          create_int_constant(4)), 
                         create_uint_constant(0xA));
    print_expr_result(ptx_constant_expressions[17], expr18);
    
    // 19. Addition with a negative literal: -10 + 20
    ptx_expr_t* expr19 = create_binary_expr(ADD, 
                         create_unary_expr(MINUS, create_int_constant(10)), 
                         create_int_constant(20));
    print_expr_result(ptx_constant_expressions[18], expr19);
    
    // 20. Addition and modulus: (10 + 3) % 7
    ptx_expr_t* expr20 = create_binary_expr(MOD, 
                         create_binary_expr(ADD, 
                                          create_int_constant(10), 
                                          create_int_constant(3)), 
                         create_int_constant(7));
    print_expr_result(ptx_constant_expressions[19], expr20);
    
    // 21. Testing operator precedence (multiplication before addition): 2 * 3 + 4 * 5
    ptx_expr_t* expr21 = create_binary_expr(ADD, 
                         create_binary_expr(MUL, 
                                          create_int_constant(2), 
                                          create_int_constant(3)), 
                         create_binary_expr(MUL, 
                                          create_int_constant(4), 
                                          create_int_constant(5)));
    print_expr_result(ptx_constant_expressions[20], expr21);
    
    // 22. Left shift and bitwise AND: (1 << 5) & 0x1F
    ptx_expr_t* expr22 = create_binary_expr(AND, 
                         create_binary_expr(SHL, 
                                          create_int_constant(1), 
                                          create_int_constant(5)), 
                         create_uint_constant(0x1F));
    print_expr_result(ptx_constant_expressions[21], expr22);
    
    // 23. Floating-point addition: 1.5 + 2.5
    ptx_expr_t* expr23 = create_binary_expr(ADD, 
                         create_float_constant(1.5), 
                         create_float_constant(2.5));
    print_expr_result(ptx_constant_expressions[22], expr23);
    
    // 24. Floating-point literal in scientific notation: 1.23e2
    ptx_expr_t* expr24 = create_float_constant(1.23e2);
    print_expr_result(ptx_constant_expressions[23], expr24);
    
    // 25. A sequence of additions: 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10
    ptx_expr_t* expr25 = create_binary_expr(ADD, 
                         create_binary_expr(ADD, 
                                          create_binary_expr(ADD, 
                                                           create_binary_expr(ADD, 
                                                                            create_binary_expr(ADD, 
                                                                                             create_binary_expr(ADD, 
                                                                                                              create_binary_expr(ADD, 
                                                                                                                               create_binary_expr(ADD, 
                                                                                                                                                create_binary_expr(ADD, 
                                                                                                                                                                 create_int_constant(1), 
                                                                                                                                                                 create_int_constant(2)), 
                                                                                                                                                create_int_constant(3)), 
                                                                                                                               create_int_constant(4)), 
                                                                                                              create_int_constant(5)), 
                                                                                             create_int_constant(6)), 
                                                                            create_int_constant(7)), 
                                                           create_int_constant(8)), 
                                          create_int_constant(9)), 
                         create_int_constant(10));
    print_expr_result(ptx_constant_expressions[24], expr25);
    
    // 26. Complex expression with multiple operations: ((1 + 2) * (3 - 4)) / (5 % 2) + (6 << 1)
    ptx_expr_t* expr26 = create_binary_expr(ADD, 
                         create_binary_expr(DIV, 
                                          create_binary_expr(MUL, 
                                                           create_binary_expr(ADD, 
                                                                            create_int_constant(1), 
                                                                            create_int_constant(2)), 
                                                           create_binary_expr(SUB, 
                                                                            create_int_constant(3), 
                                                                            create_int_constant(4))), 
                                          create_binary_expr(MOD, 
                                                           create_int_constant(5), 
                                                           create_int_constant(2))), 
                         create_binary_expr(SHL, 
                                          create_int_constant(6), 
                                          create_int_constant(1)));
    print_expr_result(ptx_constant_expressions[25], expr26);
    
    // Free all expressions to avoid memory leaks
    free_expr(expr1);
    free_expr(expr2);
    free_expr(expr3);
    free_expr(expr4);
    free_expr(expr5);
    free_expr(expr6);
    free_expr(expr7);
    free_expr(expr8);
    free_expr(expr9);
    free_expr(expr10);
    free_expr(expr11);
    free_expr(expr12);
    free_expr(expr13);
    free_expr(expr14);
    free_expr(expr15);
    free_expr(expr16);
    free_expr(expr17);
    free_expr(expr18);
    free_expr(expr19);
    free_expr(expr20);
    free_expr(expr21);
    free_expr(expr22);
    free_expr(expr23);
    free_expr(expr24);
    free_expr(expr25);
    free_expr(expr26);
    
    return 0;
} 