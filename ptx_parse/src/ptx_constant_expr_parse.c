#include <ptx_parse/ptx_constant_expr_parse.h>
#include <ptx_parse/ptx_constant_parse.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Helper function to skip whitespace
static void skip_whitespace(const char* str, int* pos) {
    while (str[*pos] && isspace(str[*pos])) {
        (*pos)++;
    }
}

// Get operator precedence
static int get_precedence(operator_t op) {
    switch (op) {
        case CONDITIONAL:
            return 1;
        case OR_LOGICAL:
            return 2;
        case AND_LOGICAL:
            return 3;
        case OR:
            return 4;
        case XOR:
            return 5;
        case AND:
            return 6;
        case EQ:
        case NE:
            return 7;
        case LT:
        case LE:
        case GT:
        case GE:
            return 8;
        case SHL:
        case SHR:
            return 9;
        case ADD:
        case SUB:
            return 10;
        case MUL:
        case DIV:
        case MOD:
            return 11;
        case PLUS:
        case MINUS:
        case NOT:
        case COMPLEMENT:
            return 12;
        case PRIMARY:
            return 13;
        default:
            return 0;
    }
}

// Check if character is part of an operator
static bool is_operator_char(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
            c == '&' || c == '|' || c == '^' || c == '~' || c == '!' ||
            c == '=' || c == '<' || c == '>' || c == '?' || c == ':');
}

// Parse binary operator
static bool parse_binary_op(const char* str, int* pos, operator_t* op) {
    skip_whitespace(str, pos);
    
    // Check for various operators
    switch (str[*pos]) {
        case '+':
            if (str[*pos + 1] == '=') {
                // += assignment not supported in constant expressions
                return false;
            }
            *op = ADD;
            (*pos)++;
            return true;
            
        case '-':
            if (str[*pos + 1] == '=') {
                // -= assignment not supported in constant expressions
                return false;
            }
            *op = SUB;
            (*pos)++;
            return true;
            
        case '*':
            if (str[*pos + 1] == '=') {
                // *= assignment not supported in constant expressions
                return false;
            }
            *op = MUL;
            (*pos)++;
            return true;
            
        case '/':
            if (str[*pos + 1] == '=') {
                // /= assignment not supported in constant expressions
                return false;
            }
            *op = DIV;
            (*pos)++;
            return true;
            
        case '%':
            if (str[*pos + 1] == '=') {
                // %= assignment not supported in constant expressions
                return false;
            }
            *op = MOD;
            (*pos)++;
            return true;
            
        case '&':
            if (str[*pos + 1] == '&') {
                *op = AND_LOGICAL;
                (*pos) += 2;
                return true;
            } else if (str[*pos + 1] == '=') {
                // &= assignment not supported in constant expressions
                return false;
            }
            *op = AND;
            (*pos)++;
            return true;
            
        case '|':
            if (str[*pos + 1] == '|') {
                *op = OR_LOGICAL;
                (*pos) += 2;
                return true;
            } else if (str[*pos + 1] == '=') {
                // |= assignment not supported in constant expressions
                return false;
            }
            *op = OR;
            (*pos)++;
            return true;
            
        case '^':
            if (str[*pos + 1] == '=') {
                // ^= assignment not supported in constant expressions
                return false;
            }
            *op = XOR;
            (*pos)++;
            return true;
            
        case '<':
            if (str[*pos + 1] == '<') {
                if (str[*pos + 2] == '=') {
                    // <<= assignment not supported in constant expressions
                    return false;
                }
                *op = SHL;
                (*pos) += 2;
                return true;
            } else if (str[*pos + 1] == '=') {
                *op = LE;
                (*pos) += 2;
                return true;
            }
            *op = LT;
            (*pos)++;
            return true;
            
        case '>':
            if (str[*pos + 1] == '>') {
                if (str[*pos + 2] == '=') {
                    // >>= assignment not supported in constant expressions
                    return false;
                }
                *op = SHR;
                (*pos) += 2;
                return true;
            } else if (str[*pos + 1] == '=') {
                *op = GE;
                (*pos) += 2;
                return true;
            }
            *op = GT;
            (*pos)++;
            return true;
            
        case '=':
            if (str[*pos + 1] == '=') {
                *op = EQ;
                (*pos) += 2;
                return true;
            }
            // Simple assignment not supported in constant expressions
            return false;
            
        case '!':
            if (str[*pos + 1] == '=') {
                *op = NE;
                (*pos) += 2;
                return true;
            }
            // Unary ! is handled elsewhere
            return false;
            
        case '?':
            // Ternary operator is handled separately
            *op = CONDITIONAL;
            (*pos)++;
            return true;
            
        default:
            return false;
    }
}

// Parse a primary expression (constant or parenthesized expression)
bool parse_primary_expr(const char* str, int* pos, ptx_expr_t** expr) {
    skip_whitespace(str, pos);
    
    if (!str[*pos]) {
        return false;  // End of string
    }
    
    // Check for parenthesized expression
    if (str[*pos] == '(') {
        (*pos)++;  // Skip opening parenthesis
        if (!parse_expr_precedence(str, pos, expr, 0)) {
            return false;
        }
        
        skip_whitespace(str, pos);
        if (str[*pos] != ')') {
            free_expr(*expr);
            *expr = NULL;
            return false;  // Missing closing parenthesis
        }
        
        (*pos)++;  // Skip closing parenthesis
        return true;
    }
    
    // Try to parse as a constant
    int start_pos = *pos;
    // Find the end of the potential constant
    while (str[*pos] && !isspace(str[*pos]) && !is_operator_char(str[*pos]) && 
           str[*pos] != '(' && str[*pos] != ')' && str[*pos] != '[' && 
           str[*pos] != ']' && str[*pos] != '{' && str[*pos] != '}' && 
           str[*pos] != ',' && str[*pos] != ';') {
        (*pos)++;
    }
    
    // Create a null-terminated substring for the constant
    int len = *pos - start_pos;
    char* constant_str = (char*)malloc(len + 1);
    if (!constant_str) {
        return false;
    }
    
    strncpy(constant_str, str + start_pos, len);
    constant_str[len] = '\0';
    
    // Try to parse as a constant
    ptx_constant_t* constant = NULL;
    bool success = parse_constant(constant_str, &constant);
    free(constant_str);
    
    if (!success) {
        return false;
    }
    
    // Create a constant expression
    *expr = create_constant_expr(*constant);
    free(constant);  // Free the constant but not its content (now owned by the expression)
    
    return *expr != NULL;
}

// Parse a unary expression (+expr, -expr, !expr, ~expr)
bool parse_unary_expr(const char* str, int* pos, ptx_expr_t** expr) {
    skip_whitespace(str, pos);
    
    operator_t op;
    bool is_unary = true;
    
    switch (str[*pos]) {
        case '+':
            op = PLUS;
            break;
        case '-':
            op = MINUS;
            break;
        case '!':
            op = NOT;
            break;
        case '~':
            op = COMPLEMENT;
            break;
        default:
            is_unary = false;
            break;
    }
    
    if (is_unary) {
        (*pos)++;  // Skip the operator
        
        ptx_expr_t* operand = NULL;
        if (!parse_unary_expr(str, pos, &operand)) {
            return false;
        }
        
        *expr = create_unary_expr(op, operand);
        return *expr != NULL;
    } else {
        // Not a unary expression, try to parse as primary
        return parse_primary_expr(str, pos, expr);
    }
}

// Parse expression using operator precedence parsing (Pratt parsing)
bool parse_expr_precedence(const char* str, int* pos, ptx_expr_t** expr, int min_precedence) {
    // Parse the left-hand side
    if (!parse_unary_expr(str, pos, expr)) {
        return false;
    }
    
    while (true) {
        skip_whitespace(str, pos);
        
        // Check if we're at the end of the string or a delimiter
        if (!str[*pos] || str[*pos] == ')' || str[*pos] == ']' || 
            str[*pos] == '}' || str[*pos] == ',' || str[*pos] == ';') {
            break;
        }
        
        // Special handling for ternary operator
        if (str[*pos] == '?') {
            // Only process if precedence is sufficient
            if (get_precedence(CONDITIONAL) < min_precedence) {
                break;
            }
            
            (*pos)++;  // Skip '?'
            
            // Parse the true expression
            ptx_expr_t* true_expr = NULL;
            if (!parse_expr_precedence(str, pos, &true_expr, 0)) {
                free_expr(*expr);
                *expr = NULL;
                return false;
            }
            
            skip_whitespace(str, pos);
            if (str[*pos] != ':') {
                free_expr(*expr);
                free_expr(true_expr);
                *expr = NULL;
                return false;  // Missing ':' in ternary operator
            }
            
            (*pos)++;  // Skip ':'
            
            // Parse the false expression
            ptx_expr_t* false_expr = NULL;
            if (!parse_expr_precedence(str, pos, &false_expr, get_precedence(CONDITIONAL))) {
                free_expr(*expr);
                free_expr(true_expr);
                *expr = NULL;
                return false;
            }
            
            // Create ternary expression
            ptx_expr_t* condition = *expr;
            *expr = create_ternary_expr(condition, true_expr, false_expr);
            if (!*expr) {
                free_expr(condition);
                free_expr(true_expr);
                free_expr(false_expr);
                return false;
            }
            
            continue;
        }
        
        // Try to parse binary operator
        int op_pos = *pos;
        operator_t op;
        if (!parse_binary_op(str, &op_pos, &op)) {
            break;
        }
        
        // Only process if precedence is sufficient
        int precedence = get_precedence(op);
        if (precedence < min_precedence) {
            break;
        }
        
        *pos = op_pos;  // Consume the operator
        
        // Parse the right-hand side with appropriate precedence
        ptx_expr_t* right = NULL;
        if (!parse_expr_precedence(str, pos, &right, precedence + 1)) {
            free_expr(*expr);
            *expr = NULL;
            return false;
        }
        
        // Create binary expression
        ptx_expr_t* left = *expr;
        *expr = create_binary_expr(op, left, right);
        if (!*expr) {
            free_expr(left);
            free_expr(right);
            return false;
        }
    }
    
    return true;
}

// Main entry point for parsing expressions
bool parse_expr(const char* str, ptx_expr_t** expr) {
    if (!str || !expr) {
        return false;
    }
    
    *expr = NULL;
    
    int pos = 0;
    skip_whitespace(str, &pos);
    
    if (!str[pos]) {
        return false;  // Empty string
    }
    
    // Parse the expression
    if (!parse_expr_precedence(str, &pos, expr, 0)) {
        return false;
    }
    
    // Make sure we consumed the entire string (except for whitespace)
    skip_whitespace(str, &pos);
    return str[pos] == '\0';
} 