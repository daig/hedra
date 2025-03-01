#include <ptx_ast/>ptx_alternate_float_format.h>
#include <ptx_ast/>ptx_type_fundamental.h>

/**
 * Helper function to convert from general ptx_type_t to ptx_alternate_float_format_t
 */
ptx_alternate_float_format_t ptx_type_to_alt_float_format(ptx_type_t type) {
    if (ptx_is_alt_float_format(type)) {
        return (ptx_alternate_float_format_t)type;
    }
    return (ptx_alternate_float_format_t)-1; // Return -1 for non-alt-float formats
}

/**
 * Get the PTX fundamental type that should be used to declare a register
 * variable containing the specified alternate format.
 */
ptx_fundamental_type_t ptx_get_storage_type_for_alt_format(ptx_alternate_float_format_t format) {
    switch (format) {
        case PTX_ALT_FLOAT_BF16:
            return PTX_FUNDAMENTAL_TYPE_B16;
        case PTX_ALT_FLOAT_E4M3:
        case PTX_ALT_FLOAT_E5M2:
        case PTX_ALT_FLOAT_UE4M3:
            return PTX_FUNDAMENTAL_TYPE_B8;
        case PTX_ALT_FLOAT_TF32:
            return PTX_FUNDAMENTAL_TYPE_B32;
        case PTX_ALT_FLOAT_E2M1:
            return PTX_FUNDAMENTAL_TYPE_B8;  // Single 4-bit value, stored in 8 bits
        case PTX_ALT_FLOAT_E2M3:
        case PTX_ALT_FLOAT_E3M2:
        case PTX_ALT_FLOAT_UE8M0:
            return PTX_FUNDAMENTAL_TYPE_B8; // Single value format
    }
    
    // Default case should not be reached if all formats are handled
    return PTX_FUNDAMENTAL_TYPE_B32;
}

/**
 * Get the number of exponent bits for the specified alternate format.
 */
int ptx_alt_format_exponent_bits(ptx_alternate_float_format_t format) {
    switch (format) {
        case PTX_ALT_FLOAT_BF16:
            return 8;
        case PTX_ALT_FLOAT_E4M3:
        case PTX_ALT_FLOAT_UE4M3:
            return 4;
        case PTX_ALT_FLOAT_E5M2:
            return 5;
        case PTX_ALT_FLOAT_TF32:
            return 8; // Same as f32
        case PTX_ALT_FLOAT_E2M1:
            return 2;
        case PTX_ALT_FLOAT_E2M3:
            return 2;
        case PTX_ALT_FLOAT_E3M2:
            return 3;
        case PTX_ALT_FLOAT_UE8M0:
            return 8;
    }
    
    return 0; // Default case
}

/**
 * Get the number of mantissa bits for the specified alternate format.
 */
int ptx_alt_format_mantissa_bits(ptx_alternate_float_format_t format) {
    switch (format) {
        case PTX_ALT_FLOAT_BF16:
            return 7;
        case PTX_ALT_FLOAT_E4M3:
        case PTX_ALT_FLOAT_UE4M3:
            return 3;
        case PTX_ALT_FLOAT_E5M2:
            return 2;
        case PTX_ALT_FLOAT_TF32:
            return 10; // Reduced precision from f32, at least 10 bits
        case PTX_ALT_FLOAT_E2M1:
            return 1;
        case PTX_ALT_FLOAT_E2M3:
            return 3;
        case PTX_ALT_FLOAT_E3M2:
            return 2;
        case PTX_ALT_FLOAT_UE8M0:
            return 0;
    }
    
    return 0; // Default case
}

/**
 * Get the total bit width of the specified alternate format.
 * For packed formats, this returns the size of a single value, not the packed size.
 */
int ptx_alt_format_bit_width(ptx_alternate_float_format_t format) {
    switch (format) {
        case PTX_ALT_FLOAT_BF16:
            return 16;
        case PTX_ALT_FLOAT_E4M3:
        case PTX_ALT_FLOAT_E5M2:
            return 8;
        case PTX_ALT_FLOAT_TF32:
            return 32;
        case PTX_ALT_FLOAT_E2M1:
            return 4;  // 4 bits (2 bits exponent + 1 bit mantissa + 1 bit sign)
        case PTX_ALT_FLOAT_E2M3:
        case PTX_ALT_FLOAT_E3M2:
            return 6;  // 6 bits
        case PTX_ALT_FLOAT_UE8M0:
            return 8;  // 8 bits
        case PTX_ALT_FLOAT_UE4M3:
            return 7;  // 7-bit format
    }
    
    return 0; // Default case
}

/**
 * Check if the specified alternate format is unsigned.
 */
bool ptx_alt_format_is_unsigned(ptx_alternate_float_format_t format) {
    return (format == PTX_ALT_FLOAT_UE8M0 || format == PTX_ALT_FLOAT_UE4M3);
}

/**
 * Check if the specified alternate format supports infinity values.
 */
bool ptx_alt_format_supports_infinity(ptx_alternate_float_format_t format) {
    // Per spec, these formats do not support infinity values
    switch (format) {
        case PTX_ALT_FLOAT_E4M3:
        case PTX_ALT_FLOAT_E2M1:
        case PTX_ALT_FLOAT_E2M3:
        case PTX_ALT_FLOAT_E3M2:
        case PTX_ALT_FLOAT_UE8M0:
        case PTX_ALT_FLOAT_UE4M3:
            return false;
        default:
            return true;
    }
}

/**
 * Check if the specified alternate format supports NaN values.
 */
bool ptx_alt_format_supports_nan(ptx_alternate_float_format_t format) {
    switch (format) {
        // These formats do not support NaN values
        case PTX_ALT_FLOAT_E2M1:
        case PTX_ALT_FLOAT_E2M3:
        case PTX_ALT_FLOAT_E3M2:
            return false;
            
        // These formats have limited NaN support
        case PTX_ALT_FLOAT_E4M3:
            // NaN values limited to 0x7f and 0xff
            return true;
        case PTX_ALT_FLOAT_UE8M0:
            // NaN value limited to 0xff
            return true;
        case PTX_ALT_FLOAT_UE4M3:
            // NaN value limited to 0x7f
            return true;
            
        // All other formats fully support NaN
        default:
            return true;
    }
}

/**
 * Check if the specified alternate format is a packed format (contains multiple values).
 */
bool ptx_alt_format_is_packed(ptx_alternate_float_format_t format) {
    // In the new system, alternate float formats are not packed
    return false;
}

/**
 * Get the packed size in bits for the specified alternate format.
 * For non-packed formats, this is the same as ptx_alt_format_bit_width().
 */
int ptx_alt_format_packed_bit_width(ptx_alternate_float_format_t format) {
    // Since no alternate formats are packed in the new system,
    // just return the bit width
    return ptx_alt_format_bit_width(format);
} 