#include <stdio.h>
#include <ptx_ast/ptx_type.h>
#include <ptx_ast/ptx_type_fundamental.h>
#include <ptx_ast/ptx_type_packed_float.h>
#include <ptx_ast/ptx_scalar_float_type.h>

// Helper function to print element type name
const char* get_element_type_name(ptx_scalar_float_type_t type) {
    switch (type) {
        case PTX_SCALAR_FLOAT_F16: return "f16";
        case PTX_SCALAR_FLOAT_F32: return "f32";
        case PTX_SCALAR_FLOAT_F64: return "f64";
        case PTX_SCALAR_FLOAT_BF16: return "bf16";
        case PTX_SCALAR_FLOAT_E4M3: return "e4m3";
        case PTX_SCALAR_FLOAT_E5M2: return "e5m2";
        case PTX_SCALAR_FLOAT_TF32: return "tf32";
        case PTX_SCALAR_FLOAT_UE4M3: return "ue4m3";
        case PTX_SCALAR_FLOAT_E2M3: return "e2m3";
        case PTX_SCALAR_FLOAT_E3M2: return "e3m2";
        case PTX_SCALAR_FLOAT_UE8M0: return "ue8m0";
        case PTX_SCALAR_FLOAT_E2M1: return "e2m1";
        default: return "unknown";
    }
}

// Helper function to print register type name
const char* get_register_type_name(ptx_register_type_t type) {
    switch (type) {
        case PTX_REGISTER_F16X2: return "f16x2";
        case PTX_REGISTER_B8: return "b8";
        case PTX_REGISTER_B16: return "b16";
        case PTX_REGISTER_B32: return "b32";
        case PTX_REGISTER_B64: return "b64";
        default: return "unknown";
    }
}

// Helper function to print fundamental type name
const char* get_fundamental_type_name(ptx_fundamental_type_t type) {
    switch (type) {
        case PTX_FUNDAMENTAL_TYPE_F16X2: return "f16x2";
        case PTX_FUNDAMENTAL_TYPE_B8: return "b8";
        case PTX_FUNDAMENTAL_TYPE_B16: return "b16";
        case PTX_FUNDAMENTAL_TYPE_B32: return "b32";
        case PTX_FUNDAMENTAL_TYPE_B64: return "b64";
        case PTX_FUNDAMENTAL_TYPE_B128: return "b128";
        default: return "other";
    }
}

// Helper function to print packed float type name
const char* get_packed_float_type_name(ptx_packed_float_type_t type) {
    // Make sure we're using the actual enum values
    if (type == PTX_PACKED_FLOAT_F16X2) return "f16x2";
    if (type == PTX_PACKED_FLOAT_F32X2) return "f32x2";
    if (type == PTX_PACKED_FLOAT_BF16X2) return "bf16x2";
    if (type == PTX_PACKED_FLOAT_E4M3X2) return "e4m3x2";
    if (type == PTX_PACKED_FLOAT_E5M2X2) return "e5m2x2";
    if (type == PTX_PACKED_FLOAT_E2M3X2) return "e2m3x2";
    if (type == PTX_PACKED_FLOAT_E3M2X2) return "e3m2x2";
    if (type == PTX_PACKED_FLOAT_UE8M0X2) return "ue8m0x2";
    if (type == PTX_PACKED_FLOAT_E2M1X2) return "e2m1x2";
    if (type == PTX_PACKED_FLOAT_E4M3X4) return "e4m3x4";
    if (type == PTX_PACKED_FLOAT_E5M2X4) return "e5m2x4";
    if (type == PTX_PACKED_FLOAT_E2M3X4) return "e2m3x4";
    if (type == PTX_PACKED_FLOAT_E3M2X4) return "e3m2x4";
    if (type == PTX_PACKED_FLOAT_E2M1X4) return "e2m1x4";
    
    // For debugging, show the enum value if it's unknown
    static char buffer[64];
    snprintf(buffer, sizeof(buffer), "unknown(%d)", type);
    return buffer;
}

int main() {
    // Print header
    printf("PTX Packed Floating Point Types Information\n");
    printf("===========================================\n\n");
    
    printf("%-10s %-10s %-12s %-12s %-10s %-15s\n", 
           "Type", "Elements", "Element Type", "Register", "Bit Size", "Fundamental");
    printf("%-10s %-10s %-12s %-12s %-10s %-15s\n", 
           "----------", "----------", "------------", "------------", "----------", "---------------");
    
    // Define an array of all the packed float types
    ptx_packed_float_type_t packed_types[] = {
        PTX_PACKED_FLOAT_F16X2,
        PTX_PACKED_FLOAT_F32X2,
        PTX_PACKED_FLOAT_BF16X2,
        PTX_PACKED_FLOAT_E4M3X2,
        PTX_PACKED_FLOAT_E5M2X2,
        PTX_PACKED_FLOAT_E2M3X2,
        PTX_PACKED_FLOAT_E3M2X2,
        PTX_PACKED_FLOAT_UE8M0X2,
        PTX_PACKED_FLOAT_E2M1X2,
        PTX_PACKED_FLOAT_E4M3X4,
        PTX_PACKED_FLOAT_E5M2X4,
        PTX_PACKED_FLOAT_E2M3X4,
        PTX_PACKED_FLOAT_E3M2X4,
        PTX_PACKED_FLOAT_E2M1X4
    };
    
    // Iterate through defined packed float types
    for (int i = 0; i < sizeof(packed_types) / sizeof(packed_types[0]); i++) {
        ptx_packed_float_type_t type = packed_types[i];
        ptx_scalar_float_type_t elem_type = ptx_packed_float_element_type(type);
        ptx_register_type_t reg_type = ptx_packed_float_register_type(type);
        ptx_fundamental_type_t fund_type = (ptx_fundamental_type_t)ptx_packed_float_to_fundamental_type(type);
        
        printf("%-10s %-10d %-12s %-12s %-10d %-15s\n",
               get_packed_float_type_name(type),
               ptx_packed_float_element_count(type),
               get_element_type_name(elem_type),
               get_register_type_name(reg_type),
               ptx_packed_float_bit_size(type),
               get_fundamental_type_name(fund_type));
    }
    
    printf("\n");
    printf("Note: Only f16x2 is a fundamental type, others are non-fundamental instruction types.\n");
    printf("For non-fundamental types, operand variables must use the corresponding bit type.\n");
    
    return 0;
} 