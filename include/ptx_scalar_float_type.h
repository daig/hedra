#pragma once

#include "ptx_type.h"

/**
 * Enum for scalar float types (fundamental and alternate, but not packed).
 * This includes both standard fundamental float types (F16, F32, F64) and
 * alternate float formats that are scalar (not packed).
 */
enum ptx_scalar_float_type {
    PTX_SCALAR_FLOAT_F16 = PTX_TYPE_F16,
    PTX_SCALAR_FLOAT_F32 = PTX_TYPE_F32,
    PTX_SCALAR_FLOAT_F64 = PTX_TYPE_F64,
    PTX_SCALAR_FLOAT_BF16 = PTX_TYPE_BF16,
    PTX_SCALAR_FLOAT_E4M3 = PTX_TYPE_E4M3,
    PTX_SCALAR_FLOAT_E5M2 = PTX_TYPE_E5M2,
    PTX_SCALAR_FLOAT_TF32 = PTX_TYPE_TF32,
    PTX_SCALAR_FLOAT_UE4M3 = PTX_TYPE_UE4M3,
    PTX_SCALAR_FLOAT_E2M1 = PTX_TYPE_E2M1,
    PTX_SCALAR_FLOAT_E2M3 = PTX_TYPE_E2M3,
    PTX_SCALAR_FLOAT_E3M2 = PTX_TYPE_E3M2,
    PTX_SCALAR_FLOAT_UE8M0 = PTX_TYPE_UE8M0
};

/**
 * Helper function to convert from general ptx_type_t to ptx_scalar_float_type_t
 * Returns the scalar float type if the input is a scalar float type, otherwise returns -1
 */
ptx_scalar_float_type_t ptx_type_to_scalar_float_type(ptx_type_t type);

/**
 * Checks if the given scalar float type is a fundamental type.
 * Returns true for F16, F32, and F64, false for alternate float formats.
 */
bool ptx_scalar_float_is_fundamental(ptx_scalar_float_type_t type);

/**
 * Checks if the given scalar float type is an alternate float format.
 * Returns false for F16, F32, and F64, true for alternate float formats.
 */
bool ptx_scalar_float_is_alternate_format(ptx_scalar_float_type_t type); 