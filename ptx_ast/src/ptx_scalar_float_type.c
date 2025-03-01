#include <ptx_ast/ptx_scalar_float_type.h>
#include <ptx_ast/ptx_type.h>

/**
 * Helper function to convert from general ptx_type_t to ptx_scalar_float_type_t
 */
ptx_scalar_float_type_t ptx_type_to_scalar_float_type(ptx_type_t type) {
    if (ptx_is_scalar_float_type(type)) {
        return (ptx_scalar_float_type_t)type;
    }
    return (ptx_scalar_float_type_t)-1; // Return -1 for non-scalar-float types
}

/**
 * Checks if the given scalar float type is a fundamental type.
 */
bool ptx_scalar_float_is_fundamental(ptx_scalar_float_type_t type) {
    return (type == PTX_SCALAR_FLOAT_F16 || 
            type == PTX_SCALAR_FLOAT_F32 || 
            type == PTX_SCALAR_FLOAT_F64);
}

/**
 * Checks if the given scalar float type is an alternate float format.
 */
bool ptx_scalar_float_is_alternate_format(ptx_scalar_float_type_t type) {
    return !ptx_scalar_float_is_fundamental(type);
} 