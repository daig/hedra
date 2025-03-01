#pragma once
#include "ptx_type.h"
#include "ptx_statespace.h"
#include <prelude/ptx_array_shape.h>


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
    ptx_type_t type;
    union {
        ptx_vector_size_t vector_size;
        ptx_array_shape_t array_shape;
    };
} ptx_shape_t;

typedef struct ptx_declaration_lhs_t {
    ptx_state_space_t statespace;
    ptx_type_t type;
    ptx_shape_t shape;
    char* name;
} ptx_declaration_type_t;

