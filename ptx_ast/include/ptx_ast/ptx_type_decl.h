#pragma once

#include <ptx_ast/ptx_type.h>
#include <ptx_ast/ptx_statespace.h>
#include <prelude/ptx_initializer_array.h>
#include <stdint.h>
#include <stddef.h>

// Maximum number of dimensions supported in PTX array declarations
#define PTX_MAX_ARRAY_DIMS 8

/**
 * Enumeration of shape kinds for PTX variables.
 */
typedef enum ptx_shape_kind {
    SHAPE_SCALAR,  // A single value
    SHAPE_VECTOR,  // A vector of values (v2 or v4)
    SHAPE_ARRAY    // A multi-dimensional array
} ptx_shape_kind_t;

/**
 * Enumeration of vector sizes supported in PTX.
 */
typedef enum ptx_vector_size { VECTOR_SIZE_V2 = 2, VECTOR_SIZE_V4 = 4 } ptx_vector_size_t;

/**
 * Enumeration of array dimensions supported in PTX.
 */
typedef enum ptx_array_size { ARRAY_SIZE_1D = 1, ARRAY_SIZE_2D = 2, ARRAY_SIZE_3D = 3, ARRAY_SIZE_4D = 4, ARRAY_SIZE_5D = 5, ARRAY_SIZE_6D = 6, ARRAY_SIZE_7D = 7, ARRAY_SIZE_8D = 8 } ptx_array_size_t;

/**
 * Structure representing the shape of a PTX variable.
 */
typedef struct ptx_shape {
    ptx_shape_kind_t kind;
    union {
        ptx_vector_size_t vector_size;
        struct {
            ptx_array_size_t size;
            int sizes[PTX_MAX_ARRAY_DIMS];
        } array;
    };
} ptx_shape_t;

/**
 * Enumeration of initializer kinds for PTX variables.
 * According to PTX specification, initializers are only allowed for constant and global state spaces.
 * Variables in constant and global state spaces with no explicit initializer are initialized to zero by default.
 * Initializers are not allowed in external variable declarations.
 * Initializers are allowed for all types except .f16, .f16x2, and .pred.
 */
typedef enum ptx_initializer_kind {
    INIT_NONE,            // No initializer
    INIT_SCALAR_INT,      // Single integer value
    INIT_SCALAR_FLOAT,    // Single floating-point value
    INIT_VECTOR,          // List of values for vector
    INIT_ARRAY,           // Nested lists for array
    INIT_VAR_ADDR,        // Address of another variable
    INIT_VAR_ADDR_OFFSET, // Address of variable plus offset
    INIT_FUNC_ADDR,       // Address of a function
    INIT_MASKED_VAR_ADDR, // Masked address of variable using mask() operator
    INIT_MASKED_INT       // Masked integer constant using mask() operator
} ptx_initializer_kind_t;

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
 * Structure representing a value in a PTX initializer.
 * This can be a scalar value, variable address, or function address.
 */
typedef struct ptx_initializer_value {
    ptx_initializer_kind_t kind;
    union {
        int64_t int_val;     // For integer scalar values
        double float_val;    // For floating-point scalar values
        struct { // Address of a variable
            const char* var_name;  
            int64_t offset;        // Optional byte offset added to address
            bool is_generic;       // Whether generic() operator is used
        } var_addr;
        const char* func_name;     // Function name for function address
        struct { // Masked value (address or integer)
            union {
                struct {
                    const char* var_name;
                    int64_t offset;
                    bool is_generic;
                } var_addr;
                int64_t int_val;
            } value;
            ptx_mask_value_t mask;  // The mask value (0xFF, 0xFF00, etc.)
        } masked;
    } value;
} ptx_initializer_value_t;

/**
 * Structure representing a complete initializer for a PTX variable.
 * Initializers are supported only for constant and global state spaces.
 * Variables that hold addresses of variables or functions should be of type .u8, .u32, or .u64.
 * Type .u8 is allowed only if the mask() operator is used.
 * Labels cannot be used in initializers.
 * PTX 3.1+ uses offsets in the global state space as default addressing for global variables,
 * rather than generic addresses. Use generic() operator explicitly for generic addresses.
 */
typedef struct ptx_initializer {
    ptx_initializer_kind_t kind;
    union {
        ptx_initializer_value_t scalar;     // For scalar initializers
        ptx_initializer_array_t* values;    // For vector/array initializers (stores ptx_initializer_value_t)
    } data;
} ptx_initializer_t;

typedef struct ptx_type_decl {
    ptx_type_t type;
    ptx_state_space_t statespace;
    ptx_shape_t shape;
    char* name;
    ptx_initializer_t initializer;
} ptx_type_decl_t;

/**
 * Example of a PTX type declaration for a constant unsigned 32-bit integer with value 42.
 * 
 * This represents the PTX declaration: .const .u32 foo = 42;
 */
static const ptx_type_decl_t example_const_u32 = {
    .type = PTX_TYPE_U32,                  // .u32 type
    .statespace = PTX_STATE_CONST,         // .const state space
    .shape = {
        .kind = SHAPE_SCALAR               // Scalar value (not vector or array)
    },
    .name = "foo",                         // Variable name
    .initializer = {
        .kind = INIT_SCALAR_INT,           // Integer initializer
        .data = {
            .scalar = {
                .kind = INIT_SCALAR_INT,   // Integer value
                .value = {
                    .int_val = 42          // Value 42
                }
            }
        }
    }
};

/**
 * Example of a PTX type declaration for a global 4-element vector of 32-bit floats.
 * 
 * This represents the PTX declaration: .global .v4 .f32 V;
 */
static const ptx_type_decl_t example_global_v4_f32 = {
    .type = PTX_TYPE_F32,                  // .f32 type
    .statespace = PTX_STATE_GLOBAL,        // .global state space
    .shape = {                             // vector shape .v4
        .kind = SHAPE_VECTOR,
        .vector_size = VECTOR_SIZE_V4
    },
    .name = "V",
    .initializer = { .kind = INIT_NONE }   // No initializer
};

/**
 * Example of a PTX type declaration for a local 2D array of 16-bit unsigned integers.
 * 
 * This represents the PTX declaration: .local .u16 kernel[19][19];
 */
static const ptx_type_decl_t example_local_u16_array = {
    .type = PTX_TYPE_U16,                  // .u16 type
    .statespace = PTX_STATE_LOCAL,         // .local state space
    .shape = {
        .kind = SHAPE_ARRAY,               // Array shape
        .array = {
            .size = ARRAY_SIZE_2D,         // 2D array
            .sizes = {19, 19}              // 19x19 dimensions
        }
    },
    .name = "kernel",                      // Variable name
    .initializer = { .kind = INIT_NONE }   // No initializer
};

/**
 * Example of a PTX type declaration for a global 2D array of 32-bit signed integers with initializer.
 * 
 * This represents the PTX declaration: .global .s32 offset[][2] = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };
 * 
 * Note: In a real implementation, this would require dynamically allocating the ptx_initializer_array_t
 * using ptx_initializer_array_create() and populating it with ptx_initializer_array_set().
 * This static example is for illustration purposes only.
 */
// This would be created dynamically in practice:
// ptx_array_shape_t shape = ptx_array_shape_create(2, 4, 2);
// ptx_initializer_array_t* array = ptx_initializer_array_create(shape, sizeof(ptx_initializer_value_t));
// ptx_initializer_value_t value = {.kind = INIT_SCALAR_INT, .value = {.int_val = -1}};
// size_t indices[2] = {0, 0};
// ptx_initializer_array_set(array, indices, &value);
// ...and so on for all elements

/**
 * Example of a PTX type declaration for a global variable initialized with the address of another variable.
 * 
 * This represents the PTX declaration: .global .u32 p1 = foo;    // offset of foo in .const space
 */
static const ptx_type_decl_t example_global_u32_var_address = {
    .type = PTX_TYPE_U32,                  // .u32 type
    .statespace = PTX_STATE_GLOBAL,        // .global state space
    .shape = {
        .kind = SHAPE_SCALAR               // Scalar value (not vector or array)
    },
    .name = "p1",                          // Variable name
    .initializer = {
        .kind = INIT_VAR_ADDR,             // Variable address initializer
        .data = {
            .scalar = {
                .kind = INIT_VAR_ADDR,     // Variable address value
                .value = {
                    .var_addr = {
                        .var_name = "foo", // Name of referenced variable
                        .offset = 0,       // No offset
                        .is_generic = false // Not using generic() operator
                    }
                }
            }
        }
    }
};

/**
 * Example of a PTX type declaration for a global vector of mixed values.
 * 
 * This represents the PTX declaration: .global .v4 .u32 mixed = {10, bar, add_func, 0};
 * 
 * Note: In a real implementation, this would require dynamically allocating the ptx_initializer_array_t
 * using ptx_initializer_array_create() and populating it with ptx_initializer_array_set().
 * This static example is for illustration purposes only.
 */
// This would be created dynamically in practice:
// ptx_array_shape_t shape = ptx_array_shape_create(1, 4);  // 1D array with 4 elements for vector
// ptx_initializer_array_t* array = ptx_initializer_array_create(shape, sizeof(ptx_initializer_value_t));
// 
// // Set value for {10}
// ptx_initializer_value_t value1 = {.kind = INIT_SCALAR_INT, .value = {.int_val = 10}};
// size_t index1[1] = {0};
// ptx_initializer_array_set(array, index1, &value1);
// 
// // Set value for {bar}
// ptx_initializer_value_t value2 = {
//     .kind = INIT_VAR_ADDR,
//     .value = {.var_addr = {.var_name = "bar", .offset = 0, .is_generic = false}}
// };
// size_t index2[1] = {1};
// ptx_initializer_array_set(array, index2, &value2);
// 
// // Set value for {add_func}
// ptx_initializer_value_t value3 = {.kind = INIT_FUNC_ADDR, .value = {.func_name = "add_func"}};
// size_t index3[1] = {2};
// ptx_initializer_array_set(array, index3, &value3);
// 
// // Set value for {0}
// ptx_initializer_value_t value4 = {.kind = INIT_SCALAR_INT, .value = {.int_val = 0}};
// size_t index4[1] = {3};
// ptx_initializer_array_set(array, index4, &value4);

