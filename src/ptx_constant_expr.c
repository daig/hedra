#include <stdlib.h>
#include "../include/ptx_constent_expr.h"

// Create a constant expression
ptx_expr_t* create_constant_expr(ptx_constant_t constant) {
    ptx_expr_t* expr = (ptx_expr_t*)malloc(sizeof(ptx_expr_t));
    if (expr == NULL) {
        return NULL;
    }
    
    expr->type = EXPR_CONSTANT;
    expr->constant = constant;
    
    return expr;
}

// Create a unary operation expression
ptx_expr_t* create_unary_expr(operator_t op, ptx_expr_t* operand) {
    if (operand == NULL) {
        return NULL;
    }
    
    ptx_expr_t* expr = (ptx_expr_t*)malloc(sizeof(ptx_expr_t));
    if (expr == NULL) {
        return NULL;
    }
    
    expr->type = EXPR_UNARY_OP;
    expr->unary.op = op;
    expr->unary.operand = operand;
    
    return expr;
}

// Create a binary operation expression
ptx_expr_t* create_binary_expr(operator_t op, ptx_expr_t* left, ptx_expr_t* right) {
    if (left == NULL || right == NULL) {
        return NULL;
    }
    
    ptx_expr_t* expr = (ptx_expr_t*)malloc(sizeof(ptx_expr_t));
    if (expr == NULL) {
        return NULL;
    }
    
    expr->type = EXPR_BINARY_OP;
    expr->binary.op = op;
    expr->binary.left = left;
    expr->binary.right = right;
    
    return expr;
}

// Create a ternary conditional expression
ptx_expr_t* create_ternary_expr(ptx_expr_t* condition, ptx_expr_t* true_expr, ptx_expr_t* false_expr) {
    if (condition == NULL || true_expr == NULL || false_expr == NULL) {
        return NULL;
    }
    
    ptx_expr_t* expr = (ptx_expr_t*)malloc(sizeof(ptx_expr_t));
    if (expr == NULL) {
        return NULL;
    }
    
    expr->type = EXPR_TERNARY_OP;
    expr->ternary.condition = condition;
    expr->ternary.true_expr = true_expr;
    expr->ternary.false_expr = false_expr;
    
    return expr;
}

// Free an expression tree
void free_expr(ptx_expr_t* expr) {
    if (expr == NULL) { return; }
    
    switch (expr->type) {
        case EXPR_UNARY_OP:
            free_expr(expr->unary.operand);
            break;
        case EXPR_BINARY_OP:
            free_expr(expr->binary.left);
            free_expr(expr->binary.right);
            break;
        case EXPR_TERNARY_OP:
            free_expr(expr->ternary.condition);
            free_expr(expr->ternary.true_expr);
            free_expr(expr->ternary.false_expr);
            break;
        case EXPR_CONSTANT:
            // No nested expressions to free
            break;
    }
    free(expr);
}

// Evaluate an expression to a constant value
bool evaluate_expr(ptx_expr_t* expr, ptx_constant_t* result) {
    if (expr == NULL || result == NULL) {
        return false;
    }
    
    // For simplicity, this initial implementation just handles constant expressions
    // A full implementation would evaluate all expression types
    if (expr->type == EXPR_CONSTANT) {
        *result = expr->constant;
        return true;
    }
    
    // TODO: Implement evaluation for other expression types
    // This would include evaluating child expressions recursively and
    // applying the respective operations on the results
    
    return false; // Cannot evaluate non-constant expressions yet
} 