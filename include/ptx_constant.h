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
    PTX_CONST_FLOAT,        // 64-bit double-precision (.f64)
    PTX_CONST_FLOAT_SINGLE, // 32-bit single-precision (.f32)
    PTX_CONST_PRED          // Predicate (True/False)
} ptx_const_type_t;

typedef struct {
    ptx_const_type_t type;
    union {
        int64_t s64_val;    // Signed integer constant (.s64)
        uint64_t u64_val;   // Unsigned integer constant (.u64)
        double f64_val;     // Double-precision floating-point constant (.f64)
        float f32_val;      // Single-precision floating-point constant (.f32)
        uint8_t pred_val;   // Predicate constant (0=False, non-zero=True)
    };
} ptx_constant_t;


// Parse integer literal string and store in ptx_constant_t
// Returns true if successful, false if invalid format
bool parse_int_literal(const char* str, ptx_constant_t* constant);

// Parse floating-point literal string and store in ptx_constant_t
// Returns true if successful, false if invalid format
bool parse_float_literal(const char* str, ptx_constant_t* constant);

// Parse predicate literal string and store in ptx_constant_t
// In PTX, integer constants may be used as predicates, with 0 as False and non-zero as True
// Returns true if successful, false if invalid format
bool parse_pred_literal(const char* str, ptx_constant_t* constant);


