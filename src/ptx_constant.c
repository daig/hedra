#include "ptx_constant.h"
#include <regex.h>
#include <string.h>  // For strlen
#include <stdlib.h>  // For strtoull, NULL

// Regex patterns for different integer literal formats
#define HEX_PATTERN "^0[xX][0-9a-fA-F]+U?$"
#define OCT_PATTERN "^0[0-7]+U?$" 
#define BIN_PATTERN "^0[bB][01]+U?$"
#define DEC_PATTERN "^[1-9][0-9]*U?$"

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