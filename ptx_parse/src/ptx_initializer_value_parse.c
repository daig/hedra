#include <ptx_parse/ptx_initializer_value_parse.h>
#include <ptx_parse/ptx_constant_expr_parse.h>
#include <ptx_parse/ptx_constant_parse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper functions for parsing

/**
 * Skip whitespace in the input string
 */
static void skip_whitespace(const char* str, int* pos) {
    while (str[*pos] != '\0' && isspace(str[*pos])) {
        (*pos)++;
    }
}

/**
 * Check if the current position in the string matches the given pattern
 */
static bool match_pattern(const char* str, int* pos, const char* pattern) {
    skip_whitespace(str, pos);
    
    size_t len = strlen(pattern);
    bool result = (strncmp(&str[*pos], pattern, len) == 0);
    
    if (result) {
        *pos += len;
    }
    
    return result;
}

/**
 * Parse an integer constant expression
 */
static bool parse_int_expr(const char* str, int* pos, int64_t* result) {
    skip_whitespace(str, pos);
    
    ptx_expr_t* expr = NULL;
    if (!parse_expr_precedence(str, pos, &expr, 0)) {
        return false;
    }
    
    // Evaluate the expression to get the constant value
    ptx_constant_t constant_result;
    if (!evaluate_expr(expr, &constant_result) || 
        (constant_result.type != PTX_CONST_INT_SIGNED && constant_result.type != PTX_CONST_INT_UNSIGNED)) {
        free_expr(expr);
        return false;
    }
    
    // Extract the integer value
    if (constant_result.type == PTX_CONST_INT_SIGNED) {
        *result = constant_result.s64_val;
    } else {
        *result = (int64_t)constant_result.u64_val;
    }
    
    free_expr(expr);
    return true;
}

/**
 * Parse an identifier string
 */
static bool parse_id_string(const char* str, int* pos, char** id_str) {
    skip_whitespace(str, pos);
    
    // Find the first non-alphanumeric, non-underscore character
    int start = *pos;
    while (str[*pos] != '\0' && (isalnum(str[*pos]) || str[*pos] == '_' || str[*pos] == '$')) {
        (*pos)++;
    }
    
    // No identifier found
    if (*pos == start) {
        return false;
    }
    
    // Copy the identifier string
    int len = *pos - start;
    *id_str = (char*)malloc(len + 1);
    if (!*id_str) {
        return false;
    }
    
    strncpy(*id_str, &str[start], len);
    (*id_str)[len] = '\0';
    
    return true;
}

// Implementation of parsing functions

bool parse_mask(const char* str, int* pos, ptx_mask_value_t* mask) {
    skip_whitespace(str, pos);
    int start_pos = *pos;
    
    printf("Attempting to parse mask: '%s' at position %d\n", &str[*pos], *pos);
    
    // Check for the fixed mask patterns first with direct string matching
    if (match_pattern(str, pos, "0xFF00000000000000")) {
        printf("  Matched mask pattern 0xFF00000000000000\n");
        *mask = MASK_BYTE7;
        return true;
    } else if (match_pattern(str, pos, "0xFF000000000000")) {
        printf("  Matched mask pattern 0xFF000000000000\n");
        *mask = MASK_BYTE6;
        return true;
    } else if (match_pattern(str, pos, "0xFF0000000000")) {
        printf("  Matched mask pattern 0xFF0000000000\n");
        *mask = MASK_BYTE5;
        return true;
    } else if (match_pattern(str, pos, "0xFF00000000")) {
        printf("  Matched mask pattern 0xFF00000000\n");
        *mask = MASK_BYTE4;
        return true;
    } else if (match_pattern(str, pos, "0xFF000000")) {
        printf("  Matched mask pattern 0xFF000000\n");
        *mask = MASK_BYTE3;
        return true;
    } else if (match_pattern(str, pos, "0xFF0000")) {
        printf("  Matched mask pattern 0xFF0000\n");
        *mask = MASK_BYTE2;
        return true;
    } else if (match_pattern(str, pos, "0xFF00")) {
        printf("  Matched mask pattern 0xFF00\n");
        *mask = MASK_BYTE1;
        return true;
    } else if (match_pattern(str, pos, "0xFF")) {
        printf("  Matched mask pattern 0xFF\n");
        *mask = MASK_BYTE0;
        return true;
    }
    
    // Reset position
    *pos = start_pos;
    
    // Alternative: try to parse as a hex constant
    // Check for "0x" prefix
    if (str[*pos] == '0' && (str[*pos + 1] == 'x' || str[*pos + 1] == 'X')) {
        printf("  Found 0x prefix\n");
        *pos += 2; // Skip the "0x" prefix
        
        // Parse the hex digits
        int hex_start = *pos;
        while (isxdigit(str[*pos])) {
            (*pos)++;
        }
        
        // Check that we have at least one hex digit
        if (*pos > hex_start) {
            // Convert the hex string to a value
            char* hex_str = (char*)malloc(*pos - start_pos + 1);
            if (hex_str) {
                strncpy(hex_str, &str[start_pos], *pos - start_pos);
                hex_str[*pos - start_pos] = '\0';
                printf("  Parsed hex string: '%s'\n", hex_str);
                
                // Try to convert to a mask value
                char* endptr;
                unsigned long long value = strtoull(hex_str, &endptr, 0);
                printf("  Converted to value: %llu\n", value);
                free(hex_str);
                
                // Determine the mask based on the value
                if (value == 0xFF) {
                    printf("  Recognized as MASK_BYTE0\n");
                    *mask = MASK_BYTE0;
                    return true;
                } else if (value == 0xFF00) {
                    printf("  Recognized as MASK_BYTE1\n");
                    *mask = MASK_BYTE1;
                    return true;
                } else if (value == 0xFF0000) {
                    printf("  Recognized as MASK_BYTE2\n");
                    *mask = MASK_BYTE2;
                    return true;
                } else if (value == 0xFF000000) {
                    printf("  Recognized as MASK_BYTE3\n");
                    *mask = MASK_BYTE3;
                    return true;
                } else if (value == 0xFF00000000ULL) {
                    printf("  Recognized as MASK_BYTE4\n");
                    *mask = MASK_BYTE4;
                    return true;
                } else if (value == 0xFF0000000000ULL) {
                    printf("  Recognized as MASK_BYTE5\n");
                    *mask = MASK_BYTE5;
                    return true;
                } else if (value == 0xFF000000000000ULL) {
                    printf("  Recognized as MASK_BYTE6\n");
                    *mask = MASK_BYTE6;
                    return true;
                } else if (value == 0xFF00000000000000ULL) {
                    printf("  Recognized as MASK_BYTE7\n");
                    *mask = MASK_BYTE7;
                    return true;
                } else {
                    printf("  Unrecognized mask value: %llu\n", value);
                }
            }
        }
    }
    
    // If we reach here, we couldn't parse the mask
    printf("  Failed to parse mask\n");
    *pos = start_pos;
    return false;
}

bool parse_masked_expr(const char* str, int* pos, ptx_initializer_value_t* value) {
    int saved_pos = *pos;
    
    printf("Attempting to parse masked expression: '%s' at position %d\n", &str[*pos], *pos);
    
    // Parse the mask
    ptx_mask_value_t mask;
    if (!parse_mask(str, pos, &mask)) {
        printf("  Failed to parse mask\n");
        *pos = saved_pos;
        return false;
    }
    
    // Parse the opening parenthesis
    if (!match_pattern(str, pos, "(")) {
        printf("  Failed to find opening parenthesis after mask\n");
        *pos = saved_pos;
        return false;
    }
    
    printf("  Found opening parenthesis after mask at position %d\n", *pos);
    
    // Find the closing parenthesis to get the full content
    int content_start = *pos;
    int paren_count = 1;
    int content_end = content_start;
    
    while (str[content_end] != '\0' && paren_count > 0) {
        if (str[content_end] == '(') {
            paren_count++;
        } else if (str[content_end] == ')') {
            paren_count--;
        }
        if (paren_count > 0) {
            content_end++;
        }
    }
    
    if (paren_count != 0) {
        printf("  Unbalanced parentheses\n");
        *pos = saved_pos;
        return false;
    }
    
    // Extract the content inside parentheses
    int content_len = content_end - content_start;
    printf("  Extracting content from position %d to %d (length %d)\n", 
           content_start, content_end, content_len);
    
    char* inner_content = (char*)malloc(content_len + 1);
    if (!inner_content) {
        printf("  Failed to allocate memory for inner content\n");
        *pos = saved_pos;
        return false;
    }
    
    strncpy(inner_content, &str[content_start], content_len);
    inner_content[content_len] = '\0';
    printf("  Inner content: '%s'\n", inner_content);
    
    // First try parsing as a complete expression
    ptx_expr_t* expr = NULL;
    if (parse_expr(inner_content, &expr)) {
        printf("  Successfully parsed inner content as expression\n");
        value->kind = INIT_VALUE_SCALAR;
        value->mask = mask;
        value->scalar_expr = expr;
        
        // Skip past the closing parenthesis
        *pos = content_end + 1;
        free(inner_content);
        return true;
    }
    
    // Reset inner parsing and try as variable address or generic address
    int inner_pos = 0;
    ptx_initializer_value_t temp_value;
    
    if (parse_var_addr_expr(inner_content, &inner_pos, &temp_value) &&
        inner_content[inner_pos] == '\0') {
        printf("  Successfully parsed inner content as variable address\n");
        // Copy the variable address from temp_value to value
        value->kind = temp_value.kind;            // INIT_VALUE_ADDR_VAR
        value->mask = mask;                       // Use the mask we parsed
        value->addr_var = temp_value.addr_var;    // Copy the address info
        
        // Skip past the closing parenthesis
        *pos = content_end + 1;
        free(inner_content);
        return true;
    }
    
    // Reset inner parsing and try as generic address
    inner_pos = 0;
    if (parse_generic_addr_expr(inner_content, &inner_pos, &temp_value) &&
        inner_content[inner_pos] == '\0') {
        printf("  Successfully parsed inner content as generic address\n");
        // Copy the variable address from temp_value to value
        value->kind = temp_value.kind;            // INIT_VALUE_ADDR_VAR
        value->mask = mask;                       // Use the mask we parsed
        value->addr_var = temp_value.addr_var;    // Copy the address info
        
        // Skip past the closing parenthesis
        *pos = content_end + 1;
        free(inner_content);
        return true;
    }
    
    // Try parsing directly with expr_precedence
    inner_pos = 0;
    if (parse_expr_precedence(inner_content, &inner_pos, &expr, 0) &&
        inner_content[inner_pos] == '\0') {
        printf("  Successfully parsed inner content with precedence parsing\n");
        value->kind = INIT_VALUE_SCALAR;
        value->mask = mask;
        value->scalar_expr = expr;
        
        // Skip past the closing parenthesis
        *pos = content_end + 1;
        free(inner_content);
        return true;
    }
    
    // If all parsing attempts failed
    printf("  Failed to parse content inside parentheses\n");
    free(inner_content);
    *pos = saved_pos;
    return false;
}

bool parse_var_addr_expr(const char* str, int* pos, ptx_initializer_value_t* value) {
    int saved_pos = *pos;
    
    // Parse the variable name (identifier)
    char* id_str = NULL;
    if (!parse_id_string(str, pos, &id_str)) {
        *pos = saved_pos;
        return false;
    }
    
    // Set up the initializer value
    value->kind = INIT_VALUE_ADDR_VAR;
    value->mask = MASK_UNMASKED;
    value->addr_var.var_name = id_str;
    value->addr_var.offset = 0;
    value->addr_var.is_generic = false;
    
    printf("  Parsing variable address: '%s' at position %d\n", &str[*pos], *pos);
    
    // Check for an offset
    if (match_pattern(str, pos, "+")) {
        printf("  Found + sign, parsing offset\n");
        // Parse the offset as a positive integer
        int64_t offset;
        if (!parse_int_expr(str, pos, &offset)) {
            printf("  Failed to parse offset\n");
            free(id_str);
            *pos = saved_pos;
            return false;
        }
        printf("  Parsed offset: %lld\n", (long long)offset);
        value->addr_var.offset = offset;
    } else if (match_pattern(str, pos, "-")) {
        printf("  Found - sign, parsing offset\n");
        // Parse the offset as a negative integer
        int64_t offset;
        if (!parse_int_expr(str, pos, &offset)) {
            printf("  Failed to parse offset\n");
            free(id_str);
            *pos = saved_pos;
            return false;
        }
        printf("  Parsed offset: -%lld\n", (long long)offset);
        value->addr_var.offset = -offset;
    }
    
    printf("  Final variable address: var_name=%s, offset=%lld\n", 
           value->addr_var.var_name, (long long)value->addr_var.offset);
    
    return true;
}

bool parse_generic_addr_expr(const char* str, int* pos, ptx_initializer_value_t* value) {
    int saved_pos = *pos;
    
    // Parse the "generic" keyword
    if (!match_pattern(str, pos, "generic")) {
        *pos = saved_pos;
        return false;
    }
    
    // Parse the opening parenthesis
    if (!match_pattern(str, pos, "(")) {
        *pos = saved_pos;
        return false;
    }
    
    // Parse the variable address expression
    if (!parse_var_addr_expr(str, pos, value)) {
        *pos = saved_pos;
        return false;
    }
    
    // Mark as generic
    value->addr_var.is_generic = true;
    
    // Parse the closing parenthesis
    if (!match_pattern(str, pos, ")")) {
        free((void*)value->addr_var.var_name);
        *pos = saved_pos;
        return false;
    }
    
    // Check for an offset after the generic expression
    if (match_pattern(str, pos, "+")) {
        // Parse the offset as a positive integer
        int64_t offset;
        if (!parse_int_expr(str, pos, &offset)) {
            free((void*)value->addr_var.var_name);
            *pos = saved_pos;
            return false;
        }
        value->addr_var.offset += offset;
    } else if (match_pattern(str, pos, "-")) {
        // Parse the offset as a negative integer
        int64_t offset;
        if (!parse_int_expr(str, pos, &offset)) {
            free((void*)value->addr_var.var_name);
            *pos = saved_pos;
            return false;
        }
        value->addr_var.offset -= offset;
    }
    
    return true;
}

bool parse_scalar_initializer(const char* str, int* pos, ptx_initializer_value_t* value, ptx_type_t data_type) {
    int saved_pos = *pos;
    
    printf("Attempting to parse: '%s' at position %d\n", str, *pos);
    
    // First check the first character to determine the most likely type of value
    char first_char = str[*pos];
    
    // Different parsing strategies based on first character
    if (first_char == '0' && (str[*pos + 1] == 'x' || str[*pos + 1] == 'X')) {
        // Likely a hex constant or mask
        
        // Try to parse as a masked expression first
        if (parse_masked_expr(str, pos, value)) {
            printf("  Parsed as masked expression\n");
            return true;
        }
        
        // Reset position
        *pos = saved_pos;
    }
    
    // If it starts with a digit, minus, plus, or decimal point, prioritize floating-point parsing
    if (isdigit(first_char) || first_char == '-' || first_char == '+' || first_char == '.') {
        // This could be a floating-point value, integer, or a negative expression
        
        // Try float parsing first
        int float_pos = *pos;
        bool has_decimal = false;
        bool has_exponent = false;
        bool has_digit = false;
        bool valid_float = false;
        
        // Parse optional sign
        if (str[float_pos] == '-' || str[float_pos] == '+') {
            printf("  Found sign: %c\n", str[float_pos]);
            float_pos++;
        }
        
        // Parse digits before decimal point
        while (isdigit(str[float_pos])) {
            has_digit = true;
            float_pos++;
        }
        printf("  Parsed digits before decimal: has_digit=%d, pos=%d\n", has_digit, float_pos);
        
        // Parse decimal point and following digits
        if (str[float_pos] == '.') {
            printf("  Found decimal point\n");
            has_decimal = true;
            float_pos++;
            while (isdigit(str[float_pos])) {
                has_digit = true;
                float_pos++;
            }
            printf("  Parsed digits after decimal: has_digit=%d, pos=%d\n", has_digit, float_pos);
        }
        
        // Parse exponent (e or E followed by optional sign and digits)
        if ((str[float_pos] == 'e' || str[float_pos] == 'E') && has_digit) {
            printf("  Found exponent marker\n");
            int exp_pos = float_pos;
            float_pos++;
            
            // Optional sign after e/E
            if (str[float_pos] == '-' || str[float_pos] == '+') {
                printf("  Found exponent sign: %c\n", str[float_pos]);
                float_pos++;
            }
            
            // Need at least one digit in exponent
            if (isdigit(str[float_pos])) {
                has_exponent = true;
                while (isdigit(str[float_pos])) {
                    float_pos++;
                }
                printf("  Parsed exponent digits: has_exponent=%d, pos=%d\n", has_exponent, float_pos);
            } else {
                // Invalid exponent, backtrack
                printf("  Invalid exponent (no digits)\n");
                float_pos = exp_pos;
            }
        }
        
        // If we found a valid floating-point number
        if (has_digit && (has_decimal || has_exponent)) {
            valid_float = true;
            int len = float_pos - *pos;
            char* float_str = (char*)malloc(len + 1);
            if (float_str) {
                strncpy(float_str, &str[*pos], len);
                float_str[len] = '\0';
                printf("  Float string: '%s'\n", float_str);
                
                // Convert to double
                char* endptr;
                double value_f64 = strtod(float_str, &endptr);
                printf("  Converted to double: %g, endptr at offset %ld\n", value_f64, endptr - float_str);
                
                // If conversion was successful
                if (endptr == float_str + len) {
                    printf("  Conversion successful\n");
                    // Create a constant for the float
                    ptx_constant_t constant;
                    
                    // Set the constant type based on the data type
                    if (data_type == PTX_TYPE_F64) {
                        constant.type = PTX_CONST_FLOAT;
                        constant.f64_val = value_f64;
                    } else {
                        // Default to F32 if not explicitly F64
                        constant.type = PTX_CONST_FLOAT_SINGLE;
                        constant.f32_val = (float)value_f64;
                    }
                    
                    // Create an expression for the constant
                    ptx_expr_t* expr = create_constant_expr(constant);
                    if (expr) {
                        // Set up the initializer value
                        value->kind = INIT_VALUE_SCALAR;
                        value->mask = MASK_UNMASKED;
                        value->scalar_expr = expr;
                        *pos = float_pos;  // Update position
                        free(float_str);
                        printf("  Successfully parsed as float\n");
                        return true;
                    } else {
                        printf("  Failed to create expression\n");
                    }
                } else {
                    printf("  Conversion failed at offset %ld\n", endptr - float_str);
                }
                
                free(float_str);
            } else {
                printf("  Failed to allocate memory for float string\n");
            }
        } else {
            printf("  Not a valid float: has_digit=%d, has_decimal=%d, has_exponent=%d\n", 
                   has_digit, has_decimal, has_exponent);
        }
        
        // If not a valid float or float parsing failed, try other numeric parsers
        if (!valid_float) {
            // Try to parse a constant expression using precedence parsing
            ptx_expr_t* expr = NULL;
            *pos = saved_pos;  // Reset position
            if (parse_expr_precedence(str, pos, &expr, 0)) {
                // Set up the initializer value
                value->kind = INIT_VALUE_SCALAR;
                value->mask = MASK_UNMASKED;
                value->scalar_expr = expr;
                printf("  Successfully parsed as constant expression\n");
                return true;
            }
            
            // Reset position
            *pos = saved_pos;
        }
    }
    
    // Check for expressions that could be complex with operators
    bool has_operators = false;
    for (int i = *pos; str[i] != '\0' && !isspace(str[i]) && str[i] != ',' && str[i] != ';'; i++) {
        if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || 
            str[i] == '&' || str[i] == '|' || str[i] == '^' || str[i] == '(' || str[i] == ')') {
            has_operators = true;
            break;
        }
    }
    
    if (has_operators) {
        // Try parsing an entire constant expression
        ptx_expr_t* expr = NULL;
        if (parse_expr(str, &expr)) {
            // Set up the initializer value
            value->kind = INIT_VALUE_SCALAR;
            value->mask = MASK_UNMASKED;
            value->scalar_expr = expr;
            // Update position to the end of string
            *pos = strlen(str);
            printf("  Successfully parsed as complex expression\n");
            return true;
        }
        // Reset position
        *pos = saved_pos;
    }
    
    // If it starts with "generic", try generic address parsing
    if (strncmp(&str[*pos], "generic", 7) == 0) {
        if (parse_generic_addr_expr(str, pos, value)) {
            printf("  Parsed as generic address\n");
            return true;
        }
        
        // Reset position
        *pos = saved_pos;
    }
    
    // If it starts with a letter or underscore, it's likely a variable address
    if (isalpha(first_char) || first_char == '_' || first_char == '$') {
        if (parse_var_addr_expr(str, pos, value)) {
            printf("  Parsed as variable address\n");
            return true;
        }
        
        // Reset position
        *pos = saved_pos;
    }
    
    // Try all other parsing strategies as fallbacks, in order from most specific to most general
    
    // Try to parse a masked expression again (fallback)
    if (parse_masked_expr(str, pos, value)) {
        printf("  Parsed as masked expression (fallback)\n");
        return true;
    }
    
    // Reset position
    *pos = saved_pos;
    
    // Try to parse a generic address expression (fallback)
    if (parse_generic_addr_expr(str, pos, value)) {
        printf("  Parsed as generic address (fallback)\n");
        return true;
    }
    
    // Reset position
    *pos = saved_pos;
    
    // Try to parse a variable address expression (fallback)
    if (parse_var_addr_expr(str, pos, value)) {
        printf("  Parsed as variable address (fallback)\n");
        return true;
    }
    
    // Reset position
    *pos = saved_pos;
    
    // Last resort: Try to parse a constant expression using precedence parsing
    ptx_expr_t* expr = NULL;
    if (parse_expr_precedence(str, pos, &expr, 0)) {
        // Set up the initializer value
        value->kind = INIT_VALUE_SCALAR;
        value->mask = MASK_UNMASKED;
        value->scalar_expr = expr;
        printf("  Parsed as constant expression (fallback)\n");
        return true;
    }
    
    // If we get here, parsing failed
    printf("  All parsing strategies failed\n");
    *pos = saved_pos;
    return false;
}

bool parse_initializer_value(const char* str, ptx_initializer_value_t* value, ptx_type_t data_type) {
    if (!str || !value) {
        return false;
    }
    
    int pos = 0;
    skip_whitespace(str, &pos);
    
    // Parse a scalar initializer value
    if (!parse_scalar_initializer(str, &pos, value, data_type)) {
        return false;
    }
    
    // Make sure we consumed the entire string (except for trailing whitespace)
    skip_whitespace(str, &pos);
    return str[pos] == '\0';
} 