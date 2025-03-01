#pragma once

#include <ptx_ast/ptx_type.h>
#include <ptx_ast/ptx_statespace.h>
#include <prelude/ptx_initializer_array.h>
#include <stdint.h>
#include <stddef.h>
#include "ptx_constant_expr.h"
/**
 * Enumeration of supported mask values for the mask() operator in PTX initializers.
 * These values specify which byte to extract from the source value.
 */
typedef enum ptx_mask_value {
    MASK_BYTE0 = 0xFF,              // Extract byte 0 (least significant)
    MASK_BYTE1 = 0xFF00,            // Extract byte 1
    MASK_BYTE2 = 0xFF0000,          // Extract byte 2
    MASK_BYTE3 = 0xFF000000,        // Extract byte 3
    MASK_BYTE4 = 0xFF00000000,      // Extract byte 4
    MASK_BYTE5 = 0xFF0000000000,    // Extract byte 5
    MASK_BYTE6 = 0xFF000000000000,  // Extract byte 6
    MASK_BYTE7 = 0xFF00000000000000, // Extract byte 7 (most significant)
    MASK_UNMASKED = 0xFFFFFFFFFFFFFFFF // Unmasked value
} ptx_mask_value_t;

typedef struct ptx_address_operand {
    const char* var_name;
    int64_t offset;
    bool is_generic;
} ptx_address_operand_t;

typedef enum ptx_intitializer_value_kind {
    INIT_VALUE_SCALAR, // float or int constant expression
    INIT_VALUE_ADDR_VAR, // address of a variable
} ptx_intitializer_value_kind_t;


/**
 * Structure representing a value in a PTX initializer.
 * This can be a scalar value, variable address, or function address.
 */
typedef struct ptx_initializer_value {
    enum {
        INIT_VALUE_SCALAR, // float or int constant expression
        INIT_VALUE_ADDR_VAR, // address of a variable
    } kind;
    ptx_mask_value_t mask;
    union {
        ptx_expr_t* scalar_expr;
        ptx_address_operand_t addr_var;
    };
} ptx_initializer_value_t;
