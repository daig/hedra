#include <ptx_ast/>ptx_type_packed_float.h>
#include <ptx_ast/>ptx_scalar_float_type.h>
#include <stdlib.h>

/**
 * Helper function to convert from general ptx_type_t to ptx_packed_float_type_t
 */
ptx_packed_float_type_t ptx_type_to_packed_float_type(ptx_type_t type) {
    switch (type) {
        case PTX_TYPE_F16X2:
            return PTX_PACKED_FLOAT_F16X2;
        case PTX_TYPE_F32X2:
            return PTX_PACKED_FLOAT_F32X2;
        case PTX_TYPE_BF16X2:
            return PTX_PACKED_FLOAT_BF16X2;
        case PTX_TYPE_E4M3X2:
            return PTX_PACKED_FLOAT_E4M3X2;
        case PTX_TYPE_E5M2X2:
            return PTX_PACKED_FLOAT_E5M2X2;
        case PTX_TYPE_E2M3X2:
            return PTX_PACKED_FLOAT_E2M3X2;
        case PTX_TYPE_E3M2X2:
            return PTX_PACKED_FLOAT_E3M2X2;
        case PTX_TYPE_UE8M0X2:
            return PTX_PACKED_FLOAT_UE8M0X2;
        case PTX_TYPE_E2M1X2:
            return PTX_PACKED_FLOAT_E2M1X2;
        case PTX_TYPE_E4M3X4:
            return PTX_PACKED_FLOAT_E4M3X4;
        case PTX_TYPE_E5M2X4:
            return PTX_PACKED_FLOAT_E5M2X4;
        case PTX_TYPE_E2M3X4:
            return PTX_PACKED_FLOAT_E2M3X4;
        case PTX_TYPE_E3M2X4:
            return PTX_PACKED_FLOAT_E3M2X4;
        case PTX_TYPE_E2M1X4:
            return PTX_PACKED_FLOAT_E2M1X4;
        default:
            return -1;
    }
}

int ptx_packed_float_element_count(ptx_packed_float_type_t type) {
    // Types ending with X4 have 4 elements, all others have 2
    switch (type) {
        case PTX_PACKED_FLOAT_E4M3X4:
        case PTX_PACKED_FLOAT_E5M2X4:
        case PTX_PACKED_FLOAT_E2M3X4:
        case PTX_PACKED_FLOAT_E3M2X4:
        case PTX_PACKED_FLOAT_E2M1X4:
            return 4;
        default:
            return 2;
    }
}

ptx_scalar_float_type_t ptx_packed_float_element_type(ptx_packed_float_type_t type) {
    switch (type) {
        case PTX_PACKED_FLOAT_F16X2:
            return PTX_SCALAR_FLOAT_F16;
        case PTX_PACKED_FLOAT_F32X2:
            return PTX_SCALAR_FLOAT_F32;
        case PTX_PACKED_FLOAT_BF16X2:
            return PTX_SCALAR_FLOAT_BF16;
        case PTX_PACKED_FLOAT_E4M3X2:
        case PTX_PACKED_FLOAT_E4M3X4:
            return PTX_SCALAR_FLOAT_E4M3;
        case PTX_PACKED_FLOAT_E5M2X2:
        case PTX_PACKED_FLOAT_E5M2X4:
            return PTX_SCALAR_FLOAT_E5M2;
        case PTX_PACKED_FLOAT_E2M3X2:
        case PTX_PACKED_FLOAT_E2M3X4:
            return PTX_SCALAR_FLOAT_E2M3;
        case PTX_PACKED_FLOAT_E3M2X2:
        case PTX_PACKED_FLOAT_E3M2X4:
            return PTX_SCALAR_FLOAT_E3M2;
        case PTX_PACKED_FLOAT_UE8M0X2:
            return PTX_SCALAR_FLOAT_UE8M0;
        case PTX_PACKED_FLOAT_E2M1X2:
        case PTX_PACKED_FLOAT_E2M1X4:
            return PTX_SCALAR_FLOAT_E2M1;
        default:
            // This should never happen if all types are covered
            return PTX_SCALAR_FLOAT_F16;
    }
}

ptx_register_type_t ptx_packed_float_register_type(ptx_packed_float_type_t type) {
    switch (type) {
        case PTX_PACKED_FLOAT_F16X2:
            return PTX_REGISTER_F16X2; // Can use either .f16x2 or .b32
        case PTX_PACKED_FLOAT_F32X2:
            return PTX_REGISTER_B64;
        case PTX_PACKED_FLOAT_BF16X2:
        case PTX_PACKED_FLOAT_E4M3X4:
        case PTX_PACKED_FLOAT_E5M2X4:
        case PTX_PACKED_FLOAT_E2M3X4:
        case PTX_PACKED_FLOAT_E3M2X4:
        case PTX_PACKED_FLOAT_E2M1X4:
            return PTX_REGISTER_B32;
        case PTX_PACKED_FLOAT_E4M3X2:
        case PTX_PACKED_FLOAT_E5M2X2:
        case PTX_PACKED_FLOAT_E2M3X2:
        case PTX_PACKED_FLOAT_E3M2X2:
        case PTX_PACKED_FLOAT_UE8M0X2:
            return PTX_REGISTER_B16;
        case PTX_PACKED_FLOAT_E2M1X2:
            return PTX_REGISTER_B8;
        default:
            // This should never happen if all types are covered
            return PTX_REGISTER_B32;
    }
}

bool ptx_packed_float_is_fundamental(ptx_packed_float_type_t type) {
    // Currently only f16x2 is a fundamental type according to the PTX documentation
    return type == PTX_PACKED_FLOAT_F16X2;
}

int ptx_packed_float_bit_size(ptx_packed_float_type_t type) {
    // Return the bit size of the register type required
    switch (ptx_packed_float_register_type(type)) {
        case PTX_REGISTER_F16X2:
        case PTX_REGISTER_B32:
            return 32;
        case PTX_REGISTER_B64:
            return 64;
        case PTX_REGISTER_B16:
            return 16;
        case PTX_REGISTER_B8:
            return 8;
        default:
            // This should never happen if all types are covered
            return 32;
    }
}

ptx_type_t ptx_register_to_fundamental_type(ptx_register_type_t reg_type) {
    switch (reg_type) {
        case PTX_REGISTER_F16X2:
            return PTX_TYPE_F16X2; // F16X2 is both a register type and a fundamental type
        case PTX_REGISTER_B8:
            return PTX_TYPE_B8;
        case PTX_REGISTER_B16:
            return PTX_TYPE_B16;
        case PTX_REGISTER_B32:
            return PTX_TYPE_B32;
        case PTX_REGISTER_B64:
            return PTX_TYPE_B64;
        default:
            // This should never happen if all types are covered
            return PTX_TYPE_B32;
    }
}

ptx_type_t ptx_packed_float_to_fundamental_type(ptx_packed_float_type_t type) {
    // If it's a fundamental type, return it directly
    if (ptx_packed_float_is_fundamental(type)) {
        return (ptx_type_t)PTX_TYPE_F16X2;
    }
    
    // Otherwise, convert the register type to a fundamental bit type
    return ptx_register_to_fundamental_type(ptx_packed_float_register_type(type));
} 