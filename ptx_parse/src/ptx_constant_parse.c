#include <ptx_parse/ptx_constant_parse.h>
#include <regex.h>
#include <string.h>  // For strlen
#include <stdlib.h>  // For strtoull, NULL
#include <math.h>    // For strtod and floating-point functions
#include <ctype.h>   // For isspace function

// Regex patterns for different integer literal formats
#define HEX_PATTERN "^0[xX][0-9a-fA-F]+U?$"
#define OCT_PATTERN "^0[0-7]+U?$" 
#define BIN_PATTERN "^0[bB][01]+U?$"
#define DEC_PATTERN "^([0-9]|[1-9][0-9]*)U?$"  // Single digit or multi-digit starting with non-zero

// Regex patterns for floating-point literals
#define FLOAT_DEC_PATTERN "^[+-]?([0-9]*[.])?[0-9]+([eE][+-]?[0-9]+)?$"
#define FLOAT_HEX_F32_PATTERN "^0[fF][0-9a-fA-F]{8}$"
#define FLOAT_HEX_F64_PATTERN "^0[dD][0-9a-fA-F]{16}$"

// Parse integer literal string and store in ptx_constant_t
// Returns true if successful, false if invalid format
bool parse_int_literal(const char* str, ptx_constant_t* constant) {
    regex_t hex_regex, oct_regex, bin_regex, dec_regex;
    bool is_unsigned = false;
    char* endptr;

    // Check if unsigned
    size_t len = strlen(str);
    if (len > 0 && str[len-1] == 'U') {
        is_unsigned = true;
    }

    // Compile regex patterns
    regcomp(&hex_regex, HEX_PATTERN, REG_EXTENDED);
    regcomp(&oct_regex, OCT_PATTERN, REG_EXTENDED);
    regcomp(&bin_regex, BIN_PATTERN, REG_EXTENDED);
    regcomp(&dec_regex, DEC_PATTERN, REG_EXTENDED);

    // Try matching each pattern
    if (regexec(&hex_regex, str, 0, NULL, 0) == 0) {
        constant->u64_val = strtoull(str, &endptr, 16);
    }
    else if (regexec(&oct_regex, str, 0, NULL, 0) == 0) {
        constant->u64_val = strtoull(str, &endptr, 8);
    }
    else if (regexec(&bin_regex, str, 0, NULL, 0) == 0) {
        // Skip "0b" prefix
        constant->u64_val = strtoull(str + 2, &endptr, 2);
    }
    else if (regexec(&dec_regex, str, 0, NULL, 0) == 0) {
        constant->u64_val = strtoull(str, &endptr, 10);
    }
    else {
        // Free regex
        regfree(&hex_regex);
        regfree(&oct_regex);
        regfree(&bin_regex);
        regfree(&dec_regex);
        return false;
    }

    // Set type based on unsigned flag
    constant->type = is_unsigned ? PTX_CONST_INT_UNSIGNED : PTX_CONST_INT_SIGNED;
    if (!is_unsigned) {
        constant->s64_val = (int64_t)constant->u64_val;
    }

    // Free regex
    regfree(&hex_regex);
    regfree(&oct_regex);
    regfree(&bin_regex);
    regfree(&dec_regex);

    return true;
}

// Helper function to convert hex to IEEE 754 float (32-bit)
float hex_to_float(const char* hex_str) {
    // Skip the '0f' or '0F' prefix
    const char* hex_digits = hex_str + 2;
    // Convert hex string to 32-bit unsigned integer
    uint32_t bits = (uint32_t)strtoull(hex_digits, NULL, 16);
    // Interpret those bits as a float
    float result;
    memcpy(&result, &bits, sizeof(float));
    return result;
}

// Helper function to convert hex to IEEE 754 double (64-bit)
double hex_to_double(const char* hex_str) {
    // Skip the '0d' or '0D' prefix
    const char* hex_digits = hex_str + 2;
    // Convert hex string to 64-bit unsigned integer
    uint64_t bits = strtoull(hex_digits, NULL, 16);
    // Interpret those bits as a double
    double result;
    memcpy(&result, &bits, sizeof(double));
    return result;
}

// Parse floating-point literal string and store in ptx_constant_t
// Returns true if successful, false if invalid format
bool parse_float_literal(const char* str, ptx_constant_t* constant) {
    regex_t float_regex, float_hex_f32_regex, float_hex_f64_regex;
    char* endptr;
    bool success = false;

    // Compile regex patterns
    regcomp(&float_regex, FLOAT_DEC_PATTERN, REG_EXTENDED);
    regcomp(&float_hex_f32_regex, FLOAT_HEX_F32_PATTERN, REG_EXTENDED);
    regcomp(&float_hex_f64_regex, FLOAT_HEX_F64_PATTERN, REG_EXTENDED);

    // Try matching each pattern
    if (regexec(&float_regex, str, 0, NULL, 0) == 0) {
        // Regular decimal floating-point format (e.g. "123.456", "1e-10")
        constant->f64_val = strtod(str, &endptr);
        constant->type = PTX_CONST_FLOAT;
        success = true;
    }
    else if (regexec(&float_hex_f32_regex, str, 0, NULL, 0) == 0) {
        // Hexadecimal 32-bit IEEE 754 format (e.g., "0F3f800000" for 1.0f)
        constant->f32_val = hex_to_float(str);
        constant->type = PTX_CONST_FLOAT_SINGLE;
        success = true;
    }
    else if (regexec(&float_hex_f64_regex, str, 0, NULL, 0) == 0) {
        // Hexadecimal 64-bit IEEE 754 format (e.g., "0D3ff0000000000000" for 1.0)
        constant->f64_val = hex_to_double(str);
        constant->type = PTX_CONST_FLOAT;
        success = true;
    }

    // Free regex
    regfree(&float_regex);
    regfree(&float_hex_f32_regex);
    regfree(&float_hex_f64_regex);

    return success;
}

// Parse predicate literal string and store in ptx_constant_t
// In PTX, integer constants may be used as predicates, with 0 as False and non-zero as True
// Returns true if successful, false if invalid format
bool parse_pred_literal(const char* str, ptx_constant_t* constant) {
    // Try to parse as an integer
    ptx_constant_t int_constant;
    if (!parse_int_literal(str, &int_constant)) {
        // If it's not a valid integer literal, fail
        return false;
    }
    
    // Convert to predicate (0 = False, non-zero = True)
    constant->type = PTX_CONST_PRED;
    
    // Get the integer value (whether it was signed or unsigned)
    uint64_t int_value = 0;
    if (int_constant.type == PTX_CONST_INT_SIGNED) {
        int_value = (int_constant.s64_val != 0) ? 1 : 0;
    } else { // PTX_CONST_INT_UNSIGNED
        int_value = (int_constant.u64_val != 0) ? 1 : 0;
    }
    
    // Set the predicate value (True = 1, False = 0)
    constant->pred_val = (uint8_t)int_value;
    
    return true;
}

/**
 * @brief Parse a string as a PTX constant (integer, float, or predicate)
 * 
 * This function attempts to parse the input string as one of the PTX constant types:
 * - Integer (decimal, hexadecimal, octal, or binary)
 * - Floating-point (decimal or hexadecimal IEEE 754)
 * - Predicate (integer value where 0=false, non-zero=true)
 *
 * @param str The string to parse as a constant
 * @param constant Pointer to ptx_constant_t* where the result will be stored
 * @return true if the string was successfully parsed as a constant, false otherwise
 */
bool parse_constant(const char* str, ptx_constant_t** constant) {
    if (!str || !constant) {
        return false;
    }
    
    // Skip leading whitespace
    const char* start = str;
    while (*start && isspace(*start)) {
        start++;
    }
    
    // Check if we have anything to parse
    if (!*start) {
        return false;
    }
    
    // Find the end of the string (ignoring trailing whitespace)
    const char* end = start + strlen(start);
    while (end > start && isspace(*(end - 1))) {
        end--;
    }
    
    // If there's no content after trimming whitespace, return false
    if (end <= start) {
        return false;
    }
    
    // Create a copy of the trimmed string
    size_t len = end - start;
    char* trimmed = (char*)malloc(len + 1);
    if (!trimmed) {
        return false; // Memory allocation failed
    }
    
    strncpy(trimmed, start, len);
    trimmed[len] = '\0';
    
    // Allocate memory for the constant structure
    *constant = (ptx_constant_t*)malloc(sizeof(ptx_constant_t));
    if (!*constant) {
        free(trimmed);
        return false; // Memory allocation failed
    }
    
    // Try integer first as it's more common
    bool result = false;
    
    if (parse_int_literal(trimmed, *constant)) {
        result = true;
    }
    // Try to parse as a float
    else if (parse_float_literal(trimmed, *constant)) {
        result = true;
    }
    
    // Free the trimmed string
    free(trimmed);
    
    // If we couldn't parse the string as a constant, free the allocated memory
    if (!result) {
        free(*constant);
        *constant = NULL;
    }
    
    return result;
} 