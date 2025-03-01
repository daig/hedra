#include "ptx_type.h"
#include "ptx_type_fundamental.h"
#include "ptx_alternate_float_format.h"
#include "ptx_type_packed_float.h"
#include "ptx_scalar_float_type.h"

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