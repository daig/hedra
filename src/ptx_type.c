#include "ptx_type.h"
#include "ptx_type_fundamental.h"
#include "ptx_alternate_float_format.h"
#include "ptx_type_packed_float.h"
#include "ptx_scalar_float_type.h"
#include "ptx_type_graphics.h"

bool ptx_is_fundamental_type(ptx_type_t type) {
    // Check if the type is within the range of fundamental types
    return (type >= PTX_TYPE_S8 && type <= PTX_TYPE_PRED) ||
           (type == PTX_TYPE_F16X2); // F16X2 is also a fundamental type
}

bool ptx_is_alt_float_format(ptx_type_t type) {
    // Check if the type is an alternate float format (non-packed)
    return (type >= PTX_TYPE_BF16 && type <= PTX_TYPE_UE8M0);
}

bool ptx_is_packed_float_type(ptx_type_t type) {
    // Check if the type is a packed float type
    return (type == PTX_TYPE_F16X2) || // The only fundamental packed type
           (type >= PTX_TYPE_F32X2 && type <= PTX_TYPE_E2M1X4); // Non-fundamental packed types
}

bool ptx_is_scalar_float_type(ptx_type_t type) {
    // Check if the type is a scalar float type
    // This includes fundamental float types (F16, F32, F64)
    // and alternate float formats (BF16, E4M3, etc.)
    // but not packed types (F16X2, F32X2, etc.)
    return ((type >= PTX_TYPE_F16 && type <= PTX_TYPE_F64) || // Fundamental scalar floats
            (type >= PTX_TYPE_BF16 && type <= PTX_TYPE_UE8M0)) && // Alternate scalar floats
           !ptx_is_packed_float_type(type); // Exclude packed types
}

/**
 * Check if a type is a packed integer type.
 */
bool ptx_is_packed_int_type(ptx_type_t type) {
    return type == PTX_TYPE_U16X2 || type == PTX_TYPE_S16X2;
}

/**
 * Check if a type is any packed type (either packed float or packed integer).
 */
bool ptx_is_packed_type(ptx_type_t type) {
    return ptx_is_packed_float_type(type) || ptx_is_packed_int_type(type);
}

ptx_type_kind_t ptx_get_type_kind(ptx_type_t type) {
    // Classify the PTX type into one of the six kind categories
    
    // Check for signed integer types
    if (type >= PTX_TYPE_S8 && type <= PTX_TYPE_S64) {
        return PTX_TYPE_KIND_SIGNED_INT;
    }
    
    // Check for packed signed integer type
    if (type == PTX_TYPE_S16X2) {
        return PTX_TYPE_KIND_SIGNED_INT;
    }
    
    // Check for unsigned integer types
    if (type >= PTX_TYPE_U8 && type <= PTX_TYPE_U64) {
        return PTX_TYPE_KIND_UNSIGNED_INT;
    }
    
    // Check for packed unsigned integer type
    if (type == PTX_TYPE_U16X2) {
        return PTX_TYPE_KIND_UNSIGNED_INT;
    }
    
    // Check for bit types
    if (type >= PTX_TYPE_B8 && type <= PTX_TYPE_B128) {
        return PTX_TYPE_KIND_BITS;
    }
    
    // Check for predicate type
    if (type == PTX_TYPE_PRED) {
        return PTX_TYPE_KIND_PRED;
    }
    
    // Check for graphics types
    if (type >= PTX_TYPE_TEXREF && type <= PTX_TYPE_SURFREF) {
        return PTX_TYPE_KIND_GRAPHICS;
    }
    
    // All remaining types are floating-point types (fundamental, alternate, or packed)
    // This includes:
    // - Fundamental scalar float types (F16, F32, F64)
    // - The fundamental packed type (F16X2)
    // - Alternate float formats (BF16, E4M3, etc.)
    // - Non-fundamental packed float types (F32X2, BF16X2, etc.)
    return PTX_TYPE_KIND_FLOAT;
} 