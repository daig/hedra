#include "ptx_constant.h"
#include <regex.h>
#include <string.h>  // For strlen
#include <stdlib.h>  // For strtoull, NULL
#include <math.h>    // For strtod and floating-point functions

// Regex patterns for different integer literal formats
#define HEX_PATTERN "^0[xX][0-9a-fA-F]+U?$"
#define OCT_PATTERN "^0[0-7]+U?$" 
#define BIN_PATTERN "^0[bB][01]+U?$"
#define DEC_PATTERN "^[1-9][0-9]*U?$"

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