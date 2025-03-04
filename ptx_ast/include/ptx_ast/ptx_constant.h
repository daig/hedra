#pragma once

#include <stdint.h>
#include <string.h>  // For strlen
#include <stdlib.h>  // For strtoull, NULL
#include <stdbool.h>

// Predefined constant for warp size
#define WARP_SZ 32

typedef enum {
    PTX_CONST_INT_SIGNED,   // Signed 64-bit integer (.s64)
    PTX_CONST_INT_UNSIGNED, // Unsigned 64-bit integer (.u64)
    PTX_CONST_FLOAT,        // 64-bit double-precision (.f64) in standard decimal format
    PTX_CONST_FLOAT_SINGLE, // 32-bit single-precision (.f32) in standard decimal format
    PTX_CONST_FLOAT_SCIENTIFIC, // Floating-point in scientific notation (e.g., 1.23e45)
    PTX_CONST_FLOAT_IEEE_HEX_F32, // 32-bit IEEE 754 hexadecimal format (e.g., 0F3f800000)
    PTX_CONST_FLOAT_IEEE_HEX_F64, // 64-bit IEEE 754 hexadecimal format (e.g., 0D3ff0000000000000)
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
    // Store the original string representation for IEEE hex formats
    char* original_str;     // Original string representation (for IEEE hex formats)
} ptx_constant_t;


