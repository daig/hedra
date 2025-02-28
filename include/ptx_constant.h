#pragma once

#include <stdint.h>
#include <string.h>  // For strlen
#include <stdlib.h>  // For strtoull, NULL
#include <regex.h>
#include <stdbool.h>

// Predefined constant for warp size
#define WARP_SZ 32

typedef enum {
    PTX_CONST_INT_SIGNED,   // Signed 64-bit integer (.s64)
    PTX_CONST_INT_UNSIGNED, // Unsigned 64-bit integer (.u64)
    PTX_CONST_FLOAT,
    PTX_CONST_PRED
} ptx_const_type_t;

typedef struct {
    ptx_const_type_t type;
    union {
        int64_t s64_val;    // Signed integer constant (.s64)
        uint64_t u64_val;   // Unsigned integer constant (.u64)
        double float_val;   // Floating-point constant 
        uint8_t pred_val;   // Predicate constant (0=False, non-zero=True)
    };
} ptx_constant_t;

// Regex patterns for different integer literal formats
#define HEX_PATTERN "^0[xX][0-9a-fA-F]+U?$"
#define OCT_PATTERN "^0[0-7]+U?$" 
#define BIN_PATTERN "^0[bB][01]+U?$"
#define DEC_PATTERN "^[1-9][0-9]*U?$"

// Parse integer literal string and store in ptx_constant_t
// Returns true if successful, false if invalid format
bool parse_int_literal(const char* str, ptx_constant_t* constant);


