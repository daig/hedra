#include <ptx_print/ptx_expr_print.h>
#include <ptx_print/ptx_constant_print.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Helper function to get the string representation of operators
static const char* get_operator_str(operator_t op) {
    switch (op) {
        case PRIMARY: return ""; // Not directly printed
        case PLUS: return "+";
        case MINUS: return "-";
        case NOT: return "!";
        case COMPLEMENT: return "~";
        case ADD: return "+";
        case SUB: return "-";
        case MUL: return "*";
        case DIV: return "/";
        case MOD: return "%";
        case AND: return "&";
        case OR: return "|";
        case XOR: return "^";
        case SHL: return "<<";
        case SHR: return ">>";
        case EQ: return "==";
        case NE: return "!=";
        case LT: return "<";
        case LE: return "<=";
        case GT: return ">";
        case GE: return ">=";
        case AND_LOGICAL: return "&&";
        case OR_LOGICAL: return "||";
        case CONDITIONAL: return "?:"; // Special case, handled separately
        default: return ""; // Unknown operator
    }
}

// Helper function to check if an operator needs parentheses based on precedence
static bool needs_parentheses(operator_t parent_op, operator_t child_op, bool is_right_child) {
    // Define precedence levels (higher value = higher precedence)
    int precedence_parent = 0;
    int precedence_child = 0;
    
    // Assign precedence levels to operators
    switch (parent_op) {
        case CONDITIONAL: precedence_parent = 1; break;
        case OR_LOGICAL: precedence_parent = 2; break;
        case AND_LOGICAL: precedence_parent = 3; break;
        case OR: precedence_parent = 4; break;
        case XOR: precedence_parent = 5; break;
        case AND: precedence_parent = 6; break;
        case EQ: case NE: precedence_parent = 7; break;
        case LT: case LE: case GT: case GE: precedence_parent = 8; break;
        case SHL: case SHR: precedence_parent = 9; break;
        case ADD: case SUB: precedence_parent = 10; break;
        case MUL: case DIV: case MOD: precedence_parent = 11; break;
        case PLUS: case MINUS: case NOT: case COMPLEMENT: precedence_parent = 12; break;
        case PRIMARY: precedence_parent = 13; break;
        default: precedence_parent = 0; break;
    }
    
    switch (child_op) {
        case CONDITIONAL: precedence_child = 1; break;
        case OR_LOGICAL: precedence_child = 2; break;
        case AND_LOGICAL: precedence_child = 3; break;
        case OR: precedence_child = 4; break;
        case XOR: precedence_child = 5; break;
        case AND: precedence_child = 6; break;
        case EQ: case NE: precedence_child = 7; break;
        case LT: case LE: case GT: case GE: precedence_child = 8; break;
        case SHL: case SHR: precedence_child = 9; break;
        case ADD: case SUB: precedence_child = 10; break;
        case MUL: case DIV: case MOD: precedence_child = 11; break;
        case PLUS: case MINUS: case NOT: case COMPLEMENT: precedence_child = 12; break;
        case PRIMARY: precedence_child = 13; break;
        default: precedence_child = 0; break;
    }
    
    // Need parentheses if child has lower precedence
    if (precedence_child < precedence_parent) {
        return true;
    }
    
    // Same precedence, but right-associative operators need parentheses when they are left children
    if (precedence_child == precedence_parent && is_right_child) {
        // These operators are right-associative
        if (parent_op == CONDITIONAL || 
            parent_op == PLUS || parent_op == MINUS || 
            parent_op == NOT || parent_op == COMPLEMENT) {
            return true;
        }
    }
    
    return false;
}

// Helper function to print an expression with appropriate parentheses
static bool print_expr_with_parens(FILE* file, ptx_expr_t* expr, operator_t parent_op, bool is_right_child) {
    operator_t expr_op = PRIMARY; // Default for constants
    
    // Determine the operator for the current expression
    if (expr->type == EXPR_UNARY_OP) {
        expr_op = expr->unary.op;
    } else if (expr->type == EXPR_BINARY_OP) {
        expr_op = expr->binary.op;
    } else if (expr->type == EXPR_TERNARY_OP) {
        expr_op = CONDITIONAL;
    }
    
    bool need_parens = needs_parentheses(parent_op, expr_op, is_right_child);
    
    if (need_parens) {
        if (fprintf(file, "(") < 0) return false;
    }
    
    bool result = print_expr_to_file(file, expr);
    
    if (need_parens) {
        if (fprintf(file, ")") < 0) return false;
    }
    
    return result;
}

// Print a constant expression to a file stream
bool print_expr_to_file(FILE* file, ptx_expr_t* expr) {
    if (!file || !expr) {
        return false;
    }
    
    switch (expr->type) {
        case EXPR_CONSTANT:
            return print_constant_to_file(file, expr->constant);
            
        case EXPR_UNARY_OP: {
            const char* op_str = get_operator_str(expr->unary.op);
            if (fprintf(file, "%s", op_str) < 0) return false;
            return print_expr_with_parens(file, expr->unary.operand, expr->unary.op, false);
        }
            
        case EXPR_BINARY_OP: {
            if (!print_expr_with_parens(file, expr->binary.left, expr->binary.op, false)) {
                return false;
            }
            
            const char* op_str = get_operator_str(expr->binary.op);
            if (fprintf(file, " %s ", op_str) < 0) return false;
            
            return print_expr_with_parens(file, expr->binary.right, expr->binary.op, true);
        }
            
        case EXPR_TERNARY_OP: {
            if (!print_expr_with_parens(file, expr->ternary.condition, CONDITIONAL, false)) {
                return false;
            }
            
            if (fprintf(file, " ? ") < 0) return false;
            
            if (!print_expr_with_parens(file, expr->ternary.true_expr, CONDITIONAL, true)) {
                return false;
            }
            
            if (fprintf(file, " : ") < 0) return false;
            
            return print_expr_with_parens(file, expr->ternary.false_expr, CONDITIONAL, true);
        }
            
        default:
            return false;
    }
}

// Print a constant expression to a string buffer using a simpler approach
int print_expr_to_buffer(char* buffer, size_t buffer_size, ptx_expr_t* expr) {
    if (!buffer || buffer_size == 0 || !expr) {
        return -1;
    }
    
    // Create a temporary file
    FILE* temp_file = tmpfile();
    if (!temp_file) {
        return -1;
    }
    
    // Print the expression to the temporary file
    if (!print_expr_to_file(temp_file, expr)) {
        fclose(temp_file);
        return -1;
    }
    
    // Reset file position to beginning of file
    rewind(temp_file);
    
    // Read the file into the buffer
    size_t bytes_read = fread(buffer, 1, buffer_size - 1, temp_file);
    buffer[bytes_read] = '\0'; // Ensure null termination
    
    // Close the temporary file
    fclose(temp_file);
    
    return bytes_read;
} 