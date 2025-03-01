#pragma once

#include "ptx_type_fundamental.h"
#include <stdbool.h>

/**
 * Alternate floating-point data formats as specified in PTX 5.2.3.
 * These cannot be used as fundamental types but are supported as source
 * or destination formats by certain instructions.
 */
typedef enum {
    PTX_ALT_FORMAT_BF16,    // 16-bit format: 8 bits exponent, 7 bits mantissa
    PTX_ALT_FORMAT_E4M3,    // 8-bit format: 4 bits exponent, 3 bits mantissa
    PTX_ALT_FORMAT_E5M2,    // 8-bit format: 5 bits exponent, 2 bits mantissa
    PTX_ALT_FORMAT_TF32,    // 32-bit format with same range as f32 but reduced precision
    PTX_ALT_FORMAT_E2M1X2,  // Packed format with two 4-bit values (2 bits exponent, 1 bit mantissa)
    PTX_ALT_FORMAT_E2M3X2,  // Packed format with two 6-bit values (2 bits exponent, 3 bits mantissa)
    PTX_ALT_FORMAT_E3M2X2,  // Packed format with two 6-bit values (3 bits exponent, 2 bits mantissa)
    PTX_ALT_FORMAT_UE8M0X2, // Packed format with two 8-bit unsigned values (8 bits exponent, 0 bits mantissa)
    PTX_ALT_FORMAT_UE4M3    // 7-bit unsigned format: 4 bits exponent, 3 bits mantissa
} ptx_alternate_float_format_t;

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