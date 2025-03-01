#pragma once

#include <ptx_ast/>ptx_type.h>
#include <stdbool.h>

/**
 * Alternate floating-point data formats as specified in PTX 5.2.3.
 * These cannot be used as fundamental types but are supported as source
 * or destination formats by certain instructions.
 */
enum ptx_alternate_float_format {
    PTX_ALT_FLOAT_BF16 = PTX_TYPE_BF16,     // 16-bit format: 8 bits exponent, 7 bits mantissa
    PTX_ALT_FLOAT_E4M3 = PTX_TYPE_E4M3,     // 8-bit format: 4 bits exponent, 3 bits mantissa
    PTX_ALT_FLOAT_E5M2 = PTX_TYPE_E5M2,     // 8-bit format: 5 bits exponent, 2 bits mantissa
    PTX_ALT_FLOAT_TF32 = PTX_TYPE_TF32,     // 32-bit format with same range as f32 but reduced precision
    PTX_ALT_FLOAT_UE4M3 = PTX_TYPE_UE4M3,   // 7-bit unsigned format: 4 bits exponent, 3 bits mantissa
    PTX_ALT_FLOAT_E2M1 = PTX_TYPE_E2M1,     // 4-bit format: 2 bits exponent, 1 bit mantissa
    PTX_ALT_FLOAT_E2M3 = PTX_TYPE_E2M3,     // 6-bit format: 2 bits exponent, 3 bits mantissa
    PTX_ALT_FLOAT_E3M2 = PTX_TYPE_E3M2,     // 6-bit format: 3 bits exponent, 2 bits mantissa
    PTX_ALT_FLOAT_UE8M0 = PTX_TYPE_UE8M0    // 8-bit unsigned format: 8 bits exponent, 0 bits mantissa
};

/**
 * Get the PTX fundamental type that should be used to declare a register
 * variable containing the specified alternate format.
 */
ptx_fundamental_type_t ptx_get_storage_type_for_alt_format(ptx_alternate_float_format_t format);

/**
 * Get the number of exponent bits for the specified alternate format.
 */
int ptx_alt_format_exponent_bits(ptx_alternate_float_format_t format);

/**
 * Get the number of mantissa bits for the specified alternate format.
 */
int ptx_alt_format_mantissa_bits(ptx_alternate_float_format_t format);

/**
 * Get the total bit width of the specified alternate format.
 * For packed formats, this returns the size of a single value, not the packed size.
 */
int ptx_alt_format_bit_width(ptx_alternate_float_format_t format);

/**
 * Check if the specified alternate format is unsigned.
 */
bool ptx_alt_format_is_unsigned(ptx_alternate_float_format_t format);

/**
 * Check if the specified alternate format supports infinity values.
 */
bool ptx_alt_format_supports_infinity(ptx_alternate_float_format_t format);

/**
 * Check if the specified alternate format supports NaN values.
 */
bool ptx_alt_format_supports_nan(ptx_alternate_float_format_t format);

/**
 * Check if the specified alternate format is a packed format (contains multiple values).
 */
bool ptx_alt_format_is_packed(ptx_alternate_float_format_t format);

/**
 * Get the packed size in bits for the specified alternate format.
 * For non-packed formats, this is the same as ptx_alt_format_bit_width().
 */
int ptx_alt_format_packed_bit_width(ptx_alternate_float_format_t format);

/**
 * Helper function to convert from general ptx_type_t to ptx_alternate_float_format_t
 * Returns the alt float format if the input is an alt float format, otherwise returns -1
 */
ptx_alternate_float_format_t ptx_type_to_alt_float_format(ptx_type_t type); 