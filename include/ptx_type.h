#pragma once

#include <stdbool.h>

// Forward declarations of enum types that will be defined in their respective headers
// This avoids circular inclusion issues
typedef enum ptx_fundamental_type ptx_fundamental_type_t;
typedef enum ptx_alternate_float_format ptx_alternate_float_format_t;
typedef enum ptx_packed_float_type ptx_packed_float_type_t;

/**
 * Enumeration of PTX type kinds based on the fundamental type categories.
 */
typedef enum {
    PTX_TYPE_KIND_SIGNED_INT,    // Signed integer types (.s8, .s16, .s32, .s64)
    PTX_TYPE_KIND_UNSIGNED_INT,  // Unsigned integer types (.u8, .u16, .u32, .u64)
    PTX_TYPE_KIND_FLOAT,         // All floating-point types (.f16, .f32, .f64, .f16x2, etc.)
    PTX_TYPE_KIND_BITS,          // Untyped bit-based types (.b8, .b16, .b32, .b64, .b128)
    PTX_TYPE_KIND_PRED           // Predicate type (.pred)
} ptx_type_kind_t;

typedef enum ptx_scalar_float_type ptx_scalar_float_type_t;

/**
 * Combined enum for all PTX types.
 * This includes fundamental types, alternate float formats, and packed float types.
 */
typedef enum {
    // Signed integer types
    PTX_TYPE_S8,
    PTX_TYPE_S16,
    PTX_TYPE_S32,
    PTX_TYPE_S64,

    // Unsigned integer types
    PTX_TYPE_U8,
    PTX_TYPE_U16,
    PTX_TYPE_U32,
    PTX_TYPE_U64,

    // Standard floating-point types (fundamental)
    PTX_TYPE_F16,
    PTX_TYPE_F32,
    PTX_TYPE_F64,

    // Bits (untyped)
    PTX_TYPE_B8,
    PTX_TYPE_B16,
    PTX_TYPE_B32,
    PTX_TYPE_B64,
    PTX_TYPE_B128,

    // Predicate type
    PTX_TYPE_PRED,

    // Packed fundamental floating-point types
    PTX_TYPE_F16X2,  // The only packed type that's also a fundamental type

    // Alternate non-packed floating-point formats
    PTX_TYPE_BF16,   // 16-bit format: 8 bits exponent, 7 bits mantissa
    PTX_TYPE_E4M3,   // 8-bit format: 4 bits exponent, 3 bits mantissa
    PTX_TYPE_E5M2,   // 8-bit format: 5 bits exponent, 2 bits mantissa
    PTX_TYPE_TF32,   // 32-bit format with same range as f32 but reduced precision
    PTX_TYPE_UE4M3,  // 7-bit unsigned format: 4 bits exponent, 3 bits mantissa
    PTX_TYPE_E2M1,   // 4-bit format: 2 bits exponent, 1 bit mantissa
    PTX_TYPE_E2M3,   // 6-bit format: 2 bits exponent, 3 bits mantissa
    PTX_TYPE_E3M2,   // 6-bit format: 3 bits exponent, 2 bits mantissa
    PTX_TYPE_UE8M0,  // 8-bit unsigned format: 8 bits exponent, 0 bits mantissa

    // Non-fundamental packed floating-point types
    PTX_TYPE_F32X2,  // Two f32 elements
    PTX_TYPE_BF16X2, // Two bf16 elements
    PTX_TYPE_E4M3X2, // Two e4m3 elements
    PTX_TYPE_E5M2X2, // Two e5m2 elements
    PTX_TYPE_E2M3X2, // Two e2m3 elements
    PTX_TYPE_E3M2X2, // Two e3m2 elements
    PTX_TYPE_UE8M0X2,// Two ue8m0 elements
    PTX_TYPE_E2M1X2, // Two e2m1 elements
    PTX_TYPE_E4M3X4, // Four e4m3 elements
    PTX_TYPE_E5M2X4, // Four e5m2 elements
    PTX_TYPE_E2M3X4, // Four e2m3 elements
    PTX_TYPE_E3M2X4, // Four e3m2 elements
    PTX_TYPE_E2M1X4,  // Four e2m1 elements

    // Packed integer types (not fundamental types, instruction types only)
    PTX_TYPE_U16X2,  // Two u16 elements packed into a 32-bit register
    PTX_TYPE_S16X2   // Two s16 elements packed into a 32-bit register
} ptx_type_t;

// The specialized type enums are now defined in their respective header files

/**
 * Check if a type is a fundamental type.
 *
 * @param type The type to check
 * @return true if the type is a fundamental type, false otherwise
 */
bool ptx_is_fundamental_type(ptx_type_t type);

/**
 * Check if a type is an alternate float format.
 *
 * @param type The type to check
 * @return true if the type is an alternate float format, false otherwise
 */
bool ptx_is_alt_float_format(ptx_type_t type);

/**
 * Check if a type is a packed float type.
 *
 * @param type The type to check
 * @return true if the type is a packed float type, false otherwise
 */
bool ptx_is_packed_float_type(ptx_type_t type);

/**
 * Check if a type is a scalar float type (fundamental or alternate, but not packed).
 *
 * @param type The type to check
 * @return true if the type is a scalar float type, false otherwise
 */
bool ptx_is_scalar_float_type(ptx_type_t type);

/**
 * Check if a type is a packed integer type.
 *
 * @param type The type to check
 * @return true if the type is a packed integer type (.u16x2 or .s16x2), false otherwise
 */
bool ptx_is_packed_int_type(ptx_type_t type);

/**
 * Check if a type is any packed type (either packed float or packed integer).
 *
 * @param type The type to check
 * @return true if the type is any packed type, false otherwise
 */
bool ptx_is_packed_type(ptx_type_t type);

/**
 * Get the kind of a PTX type.
 *
 * @param type The type to classify
 * @return The kind of the type (signed int, unsigned int, float, etc.)
 */
ptx_type_kind_t ptx_get_type_kind(ptx_type_t type); 