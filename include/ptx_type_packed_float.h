#pragma once

#include <stdbool.h>
#include "ptx_type.h"
#include "ptx_scalar_float_type.h"

/**
 * Enumeration of packed floating point types in PTX.
 * These represent register-based packed types like .f16x2, .f32x2, etc.
 */
enum ptx_packed_float_type {
    PTX_PACKED_FLOAT_F16X2 = PTX_TYPE_F16X2,    // Two half-precision floats packed into a 32-bit register
    PTX_PACKED_FLOAT_F32X2 = PTX_TYPE_F32X2,    // Two single-precision floats packed into a 64-bit register
    PTX_PACKED_FLOAT_BF16X2 = PTX_TYPE_BF16X2,   // Two BF16 floats packed into a 32-bit register
    PTX_PACKED_FLOAT_E4M3X2 = PTX_TYPE_E4M3X2,   // Two 8-bit floats (4-bit exp, 3-bit mantissa) packed into a 16-bit register
    PTX_PACKED_FLOAT_E5M2X2 = PTX_TYPE_E5M2X2,   // Two 8-bit floats (5-bit exp, 2-bit mantissa) packed into a 16-bit register
    PTX_PACKED_FLOAT_E2M3X2 = PTX_TYPE_E2M3X2,   // Two 6-bit floats (2-bit exp, 3-bit mantissa) packed into a 16-bit register
    PTX_PACKED_FLOAT_E3M2X2 = PTX_TYPE_E3M2X2,   // Two 6-bit floats (3-bit exp, 2-bit mantissa) packed into a 16-bit register
    PTX_PACKED_FLOAT_UE8M0X2 = PTX_TYPE_UE8M0X2,  // Two 8-bit floats (8-bit unsigned exp, 0-bit mantissa) packed into a 16-bit register
    PTX_PACKED_FLOAT_E2M1X2 = PTX_TYPE_E2M1X2,   // Two 4-bit floats (2-bit exp, 1-bit mantissa) packed into an 8-bit register
    PTX_PACKED_FLOAT_E4M3X4 = PTX_TYPE_E4M3X4,   // Four 8-bit floats (4-bit exp, 3-bit mantissa) packed into a 32-bit register
    PTX_PACKED_FLOAT_E5M2X4 = PTX_TYPE_E5M2X4,   // Four 8-bit floats (5-bit exp, 2-bit mantissa) packed into a 32-bit register
    PTX_PACKED_FLOAT_E2M3X4 = PTX_TYPE_E2M3X4,   // Four 6-bit floats (2-bit exp, 3-bit mantissa) packed into a 32-bit register
    PTX_PACKED_FLOAT_E3M2X4 = PTX_TYPE_E3M2X4,   // Four 6-bit floats (3-bit exp, 2-bit mantissa) packed into a 32-bit register
    PTX_PACKED_FLOAT_E2M1X4 = PTX_TYPE_E2M1X4    // Four 4-bit floats (2-bit exp, 1-bit mantissa) packed into a 16-bit register
};

/**
 * Register variable types used for packed floating point types.
 */
typedef enum {
    PTX_REGISTER_F16X2, // Can use .f16x2 register type
    PTX_REGISTER_F32X2, // Can use .f32x2 register type
    PTX_REGISTER_BF16X2, // Can use .bf16x2 register type
    PTX_REGISTER_E4M3X2,
    PTX_REGISTER_E5M2X2,
    PTX_REGISTER_E2M3X2,
    PTX_REGISTER_E3M2X2,
    PTX_REGISTER_UE8M0X2,
    PTX_REGISTER_E2M1X2,
    PTX_REGISTER_E4M3X4,
    PTX_REGISTER_E5M2X4,
    PTX_REGISTER_E2M3X4,
    PTX_REGISTER_E3M2X4,
    PTX_REGISTER_E2M1X4,
    PTX_REGISTER_B8,    // Must use .b8 register type
    PTX_REGISTER_B16,   // Must use .b16 register type
    PTX_REGISTER_B32,   // Must use .b32 register type
    PTX_REGISTER_B64    // Must use .b64 register type
} ptx_register_type_t;

/**
 * Helper function to convert from general ptx_type_t to ptx_packed_float_type_t
 * Returns the packed float type if the input is a packed float type, otherwise returns -1
 */
ptx_packed_float_type_t ptx_type_to_packed_float_type(ptx_type_t type);

/**
 * Returns the number of elements in the packed type.
 */
int ptx_packed_float_element_count(ptx_packed_float_type_t type);

/**
 * Returns the scalar float type of each element in the packed type.
 */
ptx_scalar_float_type_t ptx_packed_float_element_type(ptx_packed_float_type_t type);

/**
 * Returns the register type used for the packed type.
 */
ptx_register_type_t ptx_packed_float_register_type(ptx_packed_float_type_t type);

/**
 * Checks if the packed float type is also a fundamental type.
 */
bool ptx_packed_float_is_fundamental(ptx_packed_float_type_t type);

/**
 * Returns the bit size of the packed float type.
 */
int ptx_packed_float_bit_size(ptx_packed_float_type_t type);

/**
 * Returns the fundamental type corresponding to the register type.
 */
ptx_type_t ptx_register_to_fundamental_type(ptx_register_type_t type);

/**
 * Returns the fundamental type corresponding to the packed float type.
 */
ptx_type_t ptx_packed_float_to_fundamental_type(ptx_packed_float_type_t type); 