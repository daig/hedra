#pragma once
#include "ptx_type.h"
#include "ptx_statespace.h"
#include <prelude/ptx_array_shape.h>
#include <stdint.h>


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

/**
 * Enumeration of attribute tags supported in PTX.
 */
typedef enum ptx_attribute_tag {
    PTX_ATTR_MANAGED,  // Variable will be allocated in unified virtual memory
    PTX_ATTR_UNIFIED   // Variable/function has same memory address on host and devices
} ptx_attribute_tag_t;

/**
 * Structure representing a PTX attribute.
 */
typedef struct ptx_attribute {
    ptx_attribute_tag_t tag;
    union {
        struct {  // For PTX_ATTR_UNIFIED
            uint64_t uuid1;  // Upper 64 bits of unique identifier
            uint64_t uuid2;  // Lower 64 bits of unique identifier
        } unified;
        // PTX_ATTR_MANAGED has no additional data
    };
} ptx_attribute_t;

/**
 * Structure representing the left-hand side of a PTX declaration.
 */
typedef struct ptx_declaration_type {
    ptx_state_space_t statespace;
    ptx_type_t type;
    ptx_shape_t shape;
    char* name;
    int parameterization; // parameterized suffix. 0 means no parameterization.
    bool has_initializer;
    unsigned int alignment;  // Alignment in bytes, 0 means no alignment specified
    bool has_attribute;      // Whether this declaration has an attribute
    ptx_attribute_t attribute; // The attribute if has_attribute is true
} ptx_declaration_type_t;

