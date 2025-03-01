#include <stdlib.h>
#include "ast/ptx_constant_expr.h"

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
    
    // Temporary results for recursive evaluation of sub-expressions
    ptx_constant_t left_result, right_result, cond_result, true_result, false_result;
    
    switch (expr->type) {
        case EXPR_CONSTANT:
            // Directly copy the constant value
            *result = expr->constant;
            return true;
            
        case EXPR_UNARY_OP:
            // Evaluate the operand first
            if (!evaluate_expr(expr->unary.operand, &left_result)) {
                return false;
            }
            
            // Based on unary operator type
            switch (expr->unary.op) {
                case PLUS:  // Unary plus - preserves type
                    *result = left_result;
                    return true;
                    
                case MINUS:  // Unary minus - preserves type
                    if (left_result.type == PTX_CONST_INT_SIGNED) {
                        result->type = PTX_CONST_INT_SIGNED;
                        result->s64_val = -left_result.s64_val;
                    } else if (left_result.type == PTX_CONST_INT_UNSIGNED) {
                        result->type = PTX_CONST_INT_UNSIGNED;
                        result->u64_val = -left_result.u64_val;
                    } else if (left_result.type == PTX_CONST_FLOAT) {
                        result->type = PTX_CONST_FLOAT;
                        result->f64_val = -left_result.f64_val;
                    } else if (left_result.type == PTX_CONST_FLOAT_SINGLE) {
                        result->type = PTX_CONST_FLOAT_SINGLE;
                        result->f32_val = -left_result.f32_val;
                    } else {
                        return false;  // Cannot negate predicate
                    }
                    return true;
                    
                case NOT:  // Logical NOT - always produces a signed result (0 or 1)
                    result->type = PTX_CONST_INT_SIGNED;
                    if (left_result.type == PTX_CONST_INT_SIGNED) {
                        result->s64_val = (left_result.s64_val == 0) ? 1 : 0;
                    } else if (left_result.type == PTX_CONST_INT_UNSIGNED) {
                        result->s64_val = (left_result.u64_val == 0) ? 1 : 0;
                    } else if (left_result.type == PTX_CONST_PRED) {
                        result->s64_val = (left_result.pred_val == 0) ? 1 : 0;
                    } else {
                        return false;  // Cannot logically negate float
                    }
                    return true;
                    
                case COMPLEMENT:  // Bitwise complement - interprets operand as unsigned and produces unsigned
                    result->type = PTX_CONST_INT_UNSIGNED;
                    if (left_result.type == PTX_CONST_INT_SIGNED) {
                        result->u64_val = ~((uint64_t)left_result.s64_val);
                    } else if (left_result.type == PTX_CONST_INT_UNSIGNED) {
                        result->u64_val = ~left_result.u64_val;
                    } else {
                        return false;  // Cannot bitwise complement float or predicate
                    }
                    return true;
                    
                default:
                    return false;  // Unknown or invalid unary operator
            }
            
        case EXPR_BINARY_OP:
            // Evaluate both operands
            if (!evaluate_expr(expr->binary.left, &left_result) || 
                !evaluate_expr(expr->binary.right, &right_result)) {
                return false;
            }
            
            // Handle based on binary operator type
            switch (expr->binary.op) {
                // Arithmetic operators that follow usual arithmetic conversion
                case ADD:
                case SUB:
                case MUL:
                case DIV:
                {
                    // Usual arithmetic conversions - both operands become unsigned if either is unsigned
                    bool is_unsigned = (left_result.type == PTX_CONST_INT_UNSIGNED || 
                                        right_result.type == PTX_CONST_INT_UNSIGNED);
                    uint64_t left_val, right_val;
                    
                    // Convert operands to the common type
                    if (left_result.type == PTX_CONST_INT_SIGNED) {
                        left_val = (uint64_t)left_result.s64_val;
                    } else if (left_result.type == PTX_CONST_INT_UNSIGNED) {
                        left_val = left_result.u64_val;
                    } else {
                        return false;  // Non-integer operand
                    }
                    
                    if (right_result.type == PTX_CONST_INT_SIGNED) {
                        right_val = (uint64_t)right_result.s64_val;
                    } else if (right_result.type == PTX_CONST_INT_UNSIGNED) {
                        right_val = right_result.u64_val;
                    } else {
                        return false;  // Non-integer operand
                    }
                    
                    // Perform the operation and set result type
                    result->type = is_unsigned ? PTX_CONST_INT_UNSIGNED : PTX_CONST_INT_SIGNED;
                    
                    if (expr->binary.op == ADD) {
                        if (is_unsigned) {
                            result->u64_val = left_val + right_val;
                        } else {
                            result->s64_val = (int64_t)left_val + (int64_t)right_val;
                        }
                    } 
                    else if (expr->binary.op == SUB) {
                        if (is_unsigned) {
                            result->u64_val = left_val - right_val;
                        } else {
                            result->s64_val = (int64_t)left_val - (int64_t)right_val;
                        }
                    }
                    else if (expr->binary.op == MUL) {
                        if (is_unsigned) {
                            result->u64_val = left_val * right_val;
                        } else {
                            result->s64_val = (int64_t)left_val * (int64_t)right_val;
                        }
                    }
                    else if (expr->binary.op == DIV) {
                        if (right_val == 0) {
                            return false;  // Division by zero
                        }
                        
                        if (is_unsigned) {
                            result->u64_val = left_val / right_val;
                        } else {
                            result->s64_val = (int64_t)left_val / (int64_t)right_val;
                        }
                    }
                    return true;
                }
                
                // Remainder (%) always treats operands as unsigned
                case MOD:
                {
                    uint64_t left_val, right_val;
                    
                    // Convert operands to unsigned
                    if (left_result.type == PTX_CONST_INT_SIGNED) {
                        left_val = (uint64_t)left_result.s64_val;
                    } else if (left_result.type == PTX_CONST_INT_UNSIGNED) {
                        left_val = left_result.u64_val;
                    } else {
                        return false;  // Non-integer operand
                    }
                    
                    if (right_result.type == PTX_CONST_INT_SIGNED) {
                        right_val = (uint64_t)right_result.s64_val;
                    } else if (right_result.type == PTX_CONST_INT_UNSIGNED) {
                        right_val = right_result.u64_val;
                    } else {
                        return false;  // Non-integer operand
                    }
                    
                    if (right_val == 0) {
                        return false;  // Division by zero
                    }
                    
                    // Modulo always produces unsigned result
                    result->type = PTX_CONST_INT_UNSIGNED;
                    result->u64_val = left_val % right_val;
                    return true;
                }
                
                // Shift operators (<<, >>) - second operand is always unsigned
                case SHL:
                case SHR:
                {
                    uint64_t shift_amount;
                    
                    // Convert right operand to unsigned
                    if (right_result.type == PTX_CONST_INT_SIGNED) {
                        shift_amount = (uint64_t)right_result.s64_val;
                    } else if (right_result.type == PTX_CONST_INT_UNSIGNED) {
                        shift_amount = right_result.u64_val;
                    } else {
                        return false;  // Non-integer operand
                    }
                    
                    // Result type is same as first operand
                    result->type = left_result.type;
                    
                    if (expr->binary.op == SHL) {
                        // Left shift
                        if (left_result.type == PTX_CONST_INT_SIGNED) {
                            result->s64_val = left_result.s64_val << shift_amount;
                        } else if (left_result.type == PTX_CONST_INT_UNSIGNED) {
                            result->u64_val = left_result.u64_val << shift_amount;
                        } else {
                            return false;  // Non-integer operand
                        }
                    } 
                    else if (expr->binary.op == SHR) {
                        // Right shift (arithmetic for signed, logical for unsigned)
                        if (left_result.type == PTX_CONST_INT_SIGNED) {
                            result->s64_val = left_result.s64_val >> shift_amount;
                        } else if (left_result.type == PTX_CONST_INT_UNSIGNED) {
                            result->u64_val = left_result.u64_val >> shift_amount;
                        } else {
                            return false;  // Non-integer operand
                        }
                    }
                    return true;
                }
                
                // Bitwise operators follow usual arithmetic conversion
                case AND:
                case OR:
                case XOR:
                {
                    // Usual arithmetic conversions - both operands become unsigned if either is unsigned
                    bool is_unsigned = (left_result.type == PTX_CONST_INT_UNSIGNED || 
                                        right_result.type == PTX_CONST_INT_UNSIGNED);
                    uint64_t left_val, right_val;
                    
                    // Convert operands to the common type
                    if (left_result.type == PTX_CONST_INT_SIGNED) {
                        left_val = (uint64_t)left_result.s64_val;
                    } else if (left_result.type == PTX_CONST_INT_UNSIGNED) {
                        left_val = left_result.u64_val;
                    } else {
                        return false;  // Non-integer operand
                    }
                    
                    if (right_result.type == PTX_CONST_INT_SIGNED) {
                        right_val = (uint64_t)right_result.s64_val;
                    } else if (right_result.type == PTX_CONST_INT_UNSIGNED) {
                        right_val = right_result.u64_val;
                    } else {
                        return false;  // Non-integer operand
                    }
                    
                    // Set result type
                    result->type = is_unsigned ? PTX_CONST_INT_UNSIGNED : PTX_CONST_INT_SIGNED;
                    
                    // Perform the bitwise operation
                    if (expr->binary.op == AND) {
                        if (is_unsigned) {
                            result->u64_val = left_val & right_val;
                        } else {
                            result->s64_val = (int64_t)(left_val & right_val);
                        }
                    } 
                    else if (expr->binary.op == OR) {
                        if (is_unsigned) {
                            result->u64_val = left_val | right_val;
                        } else {
                            result->s64_val = (int64_t)(left_val | right_val);
                        }
                    }
                    else if (expr->binary.op == XOR) {
                        if (is_unsigned) {
                            result->u64_val = left_val ^ right_val;
                        } else {
                            result->s64_val = (int64_t)(left_val ^ right_val);
                        }
                    }
                    return true;
                }
                
                // Comparison operators produce signed result (0 or 1)
                case EQ:
                case NE:
                case LT:
                case LE:
                case GT:
                case GE:
                {
                    // Usual arithmetic conversions for ordered comparisons
                    bool is_unsigned = (left_result.type == PTX_CONST_INT_UNSIGNED || 
                                        right_result.type == PTX_CONST_INT_UNSIGNED);
                    uint64_t left_val, right_val;
                    
                    // Convert operands to the common type
                    if (left_result.type == PTX_CONST_INT_SIGNED) {
                        left_val = (uint64_t)left_result.s64_val;
                    } else if (left_result.type == PTX_CONST_INT_UNSIGNED) {
                        left_val = left_result.u64_val;
                    } else {
                        return false;  // Non-integer operand
                    }
                    
                    if (right_result.type == PTX_CONST_INT_SIGNED) {
                        right_val = (uint64_t)right_result.s64_val;
                    } else if (right_result.type == PTX_CONST_INT_UNSIGNED) {
                        right_val = right_result.u64_val;
                    } else {
                        return false;  // Non-integer operand
                    }
                    
                    // Result is signed (0 or 1)
                    result->type = PTX_CONST_INT_SIGNED;
                    
                    // Perform the comparison based on whether operands are treated as signed or unsigned
                    if (expr->binary.op == EQ) {
                        result->s64_val = (left_val == right_val) ? 1 : 0;
                    } 
                    else if (expr->binary.op == NE) {
                        result->s64_val = (left_val != right_val) ? 1 : 0;
                    }
                    else if (expr->binary.op == LT) {
                        if (is_unsigned) {
                            result->s64_val = (left_val < right_val) ? 1 : 0;
                        } else {
                            result->s64_val = ((int64_t)left_val < (int64_t)right_val) ? 1 : 0;
                        }
                    }
                    else if (expr->binary.op == LE) {
                        if (is_unsigned) {
                            result->s64_val = (left_val <= right_val) ? 1 : 0;
                        } else {
                            result->s64_val = ((int64_t)left_val <= (int64_t)right_val) ? 1 : 0;
                        }
                    }
                    else if (expr->binary.op == GT) {
                        if (is_unsigned) {
                            result->s64_val = (left_val > right_val) ? 1 : 0;
                        } else {
                            result->s64_val = ((int64_t)left_val > (int64_t)right_val) ? 1 : 0;
                        }
                    }
                    else if (expr->binary.op == GE) {
                        if (is_unsigned) {
                            result->s64_val = (left_val >= right_val) ? 1 : 0;
                        } else {
                            result->s64_val = ((int64_t)left_val >= (int64_t)right_val) ? 1 : 0;
                        }
                    }
                    return true;
                }
                
                // Logical operators produce signed result (0 or 1)
                case AND_LOGICAL:
                case OR_LOGICAL:
                {
                    bool left_bool, right_bool;
                    
                    // Convert operands to boolean values
                    if (left_result.type == PTX_CONST_INT_SIGNED) {
                        left_bool = (left_result.s64_val != 0);
                    } else if (left_result.type == PTX_CONST_INT_UNSIGNED) {
                        left_bool = (left_result.u64_val != 0);
                    } else if (left_result.type == PTX_CONST_PRED) {
                        left_bool = (left_result.pred_val != 0);
                    } else {
                        return false;  // Non-integer/predicate operand
                    }
                    
                    if (right_result.type == PTX_CONST_INT_SIGNED) {
                        right_bool = (right_result.s64_val != 0);
                    } else if (right_result.type == PTX_CONST_INT_UNSIGNED) {
                        right_bool = (right_result.u64_val != 0);
                    } else if (right_result.type == PTX_CONST_PRED) {
                        right_bool = (right_result.pred_val != 0);
                    } else {
                        return false;  // Non-integer/predicate operand
                    }
                    
                    // Result is signed (0 or 1)
                    result->type = PTX_CONST_INT_SIGNED;
                    
                    if (expr->binary.op == AND_LOGICAL) {
                        result->s64_val = (left_bool && right_bool) ? 1 : 0;
                    } else { // OR_LOGICAL
                        result->s64_val = (left_bool || right_bool) ? 1 : 0;
                    }
                    return true;
                }
                
                default:
                    return false;  // Unknown or invalid binary operator
            }
            
        case EXPR_TERNARY_OP:
            // Evaluate the condition first
            if (!evaluate_expr(expr->ternary.condition, &cond_result)) {
                return false;
            }
            
            // Convert condition to boolean
            bool condition_bool;
            if (cond_result.type == PTX_CONST_INT_SIGNED) {
                condition_bool = (cond_result.s64_val != 0);
            } else if (cond_result.type == PTX_CONST_INT_UNSIGNED) {
                condition_bool = (cond_result.u64_val != 0);
            } else if (cond_result.type == PTX_CONST_PRED) {
                condition_bool = (cond_result.pred_val != 0);
            } else {
                return false;  // Non-integer/predicate condition
            }
            
            // Evaluate only the branch that will be taken
            if (condition_bool) {
                if (!evaluate_expr(expr->ternary.true_expr, &true_result)) {
                    return false;
                }
                *result = true_result;
            } else {
                if (!evaluate_expr(expr->ternary.false_expr, &false_result)) {
                    return false;
                }
                *result = false_result;
            }
            
            // If both branches were evaluated, perform usual arithmetic conversions
            // For now we're doing lazy evaluation to avoid unnecessary work
            return true;
            
        default:
            return false;  // Unknown expression type
    }
} 