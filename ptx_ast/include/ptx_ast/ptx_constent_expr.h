#pragma once
#include <ptx_ast/ptx_constant.h>

typedef enum {
    // Primary operator
    PRIMARY, // ()
    // Unary operators
    PLUS,       // +
    MINUS,      // -
    NOT,        // !
    COMPLEMENT, // ~
    // Binary operators
    ADD,        // +
    SUB,        // -
    MUL,        // *
    DIV,        // /
    MOD,        // %
    AND,        // &
    OR,         // |
    XOR,        // ^
    SHL,        // <<
    SHR,        // >>
    // Comparison operators
    EQ,         // ==
    NE,         // !=
    LT,         // <
    LE,         // <=
    GT,         // >
    GE,         // >=
    // Logical operators
    AND_LOGICAL, // &&
    OR_LOGICAL,  // ||
    // Ternary conditional operator
    CONDITIONAL  // ?:
} operator_t;

typedef struct ptx_expr ptx_expr_t;

// Structure representing a node in the expression tree
struct ptx_expr {
    enum {
        EXPR_CONSTANT,    // Literal constant value
        EXPR_UNARY_OP,    // Unary operation
        EXPR_BINARY_OP,   // Binary operation
        EXPR_TERNARY_OP   // Ternary operation (? :)
    } type;
    
    union {
        // For constant literals
        ptx_constant_t constant;
        
        // For unary operations
        struct {
            operator_t op;
            ptx_expr_t* operand;
        } unary;
        
        // For binary operations
        struct {
            operator_t op;
            ptx_expr_t* left;
            ptx_expr_t* right;
        } binary;
        
        // For ternary operations (? :)
        struct {
            ptx_expr_t* condition;
            ptx_expr_t* true_expr;
            ptx_expr_t* false_expr;
        } ternary;
    };
};

// Create a constant expression
ptx_expr_t* create_constant_expr(ptx_constant_t constant);

// Create a unary operation expression
ptx_expr_t* create_unary_expr(operator_t op, ptx_expr_t* operand);

// Create a binary operation expression
ptx_expr_t* create_binary_expr(operator_t op, ptx_expr_t* left, ptx_expr_t* right);

// Create a ternary conditional expression
ptx_expr_t* create_ternary_expr(ptx_expr_t* condition, ptx_expr_t* true_expr, ptx_expr_t* false_expr);

// Free an expression tree
void free_expr(ptx_expr_t* expr);

// Evaluate an expression to a constant value
bool evaluate_expr(ptx_expr_t* expr, ptx_constant_t* result);

