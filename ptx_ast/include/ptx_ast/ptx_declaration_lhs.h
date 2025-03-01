#pragma once
#include "ptx_type.h"
#include "ptx_statespace.h"
#include <prelude/ptx_array_shape.h>


/**
 * Enumeration of supported mask values for masked addresses.
 * The mask() operator extracts n consecutive bits from an expression and inserts
 * these bits at the lowest position of the initialized variable.
 * Available since PTX ISA version 7.1 for variable addresses and since 
 * PTX ISA version 7.3 for integer constant expressions.
 * Mask operator can only extract a single byte starting at byte boundary.
 */
typedef enum ptx_mask_value {
    PTX_MASK_BYTE0 = 0xFF,                   // 0xFF - Extract byte 0
    PTX_MASK_BYTE1 = 0xFF00,                 // 0xFF00 - Extract byte 1
    PTX_MASK_BYTE2 = 0xFF0000,               // 0xFF0000 - Extract byte 2
    PTX_MASK_BYTE3 = 0xFF000000,             // 0xFF000000 - Extract byte 3
    PTX_MASK_BYTE4 = 0xFF00000000,           // 0xFF00000000 - Extract byte 4
    PTX_MASK_BYTE5 = 0xFF0000000000,         // 0xFF0000000000 - Extract byte 5
    PTX_MASK_BYTE6 = 0xFF000000000000,       // 0xFF000000000000 - Extract byte 6
    PTX_MASK_BYTE7 = 0xFF00000000000000,     // 0xFF00000000000000 - Extract byte 7
    PTX_UNMASKED = 0xFFFFFFFFFFFFFFFF        // 0xFFFFFFFFFFFFFFFF - No mask
} ptx_mask_value_t;

/**
 * Enumeration of vector sizes supported in PTX.
 */
typedef enum ptx_vector_size { VECTOR_SIZE_V2 = 2, VECTOR_SIZE_V4 = 4 } ptx_vector_size_t;
/**
 * Enumeration of shape kinds for PTX variables.
 */
typedef enum ptx_shape_kind {
    SHAPE_SCALAR,  // A single value
    SHAPE_VECTOR,  // A vector of values (v2 or v4)
    SHAPE_ARRAY    // A multi-dimensional array
} ptx_shape_kind_t;

/**
 * Structure representing the shape of a PTX variable.
 */
typedef struct ptx_shape {
    ptx_shape_kind_t kind;
    union {
        ptx_vector_size_t vector_size;
        ptx_array_shape_t array_shape;
    };
} ptx_shape_t;

typedef struct ptx_declaration_type_t {
    ptx_state_space_t statespace;
    ptx_type_t type;
    ptx_shape_t shape;
    char* name;
} ptx_declaration_type_t;

