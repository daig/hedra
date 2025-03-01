#include <stdio.h>
#include <ptx_ast/ptx_type.h>
#include <ptx_ast/ptx_type_fundamental.h>
#include <ptx_ast/ptx_alternate_float_format.h>
#include <ptx_ast/ptx_type_packed_float.h>
#include <ptx_ast/ptx_scalar_float_type.h>

// Helper function to print type name
const char* get_type_name(ptx_type_t type) {
    static const char* type_names[] = {
        "s8", "s16", "s32", "s64",
        "u8", "u16", "u32", "u64",
        "f16", "f32", "f64",
        "b8", "b16", "b32", "b64", "b128",
        "pred",
        "f16x2",
        "bf16", "e4m3", "e5m2", "tf32", "ue4m3", "e2m1", "e2m3", "e3m2", "ue8m0",
        "f32x2", "bf16x2", "e4m3x2", "e5m2x2", "e2m3x2", "e3m2x2", "ue8m0x2", "e2m1x2",
        "e4m3x4", "e5m2x4", "e2m3x4", "e3m2x4", "e2m1x4"
    };
    
    if (type >= 0 && type < sizeof(type_names) / sizeof(type_names[0])) {
        return type_names[type];
    }
    return "unknown";
}

// Test function to print type classification
void test_type_classification(ptx_type_t type) {
    printf("Type %-8s: ", get_type_name(type));
    
    if (ptx_is_fundamental_type(type)) {
        printf("Fundamental ");
    }
    
    if (ptx_is_alt_float_format(type)) {
        printf("Alt-Float ");
    }
    
    if (ptx_is_packed_float_type(type)) {
        printf("Packed ");
    }
    
    if (ptx_is_scalar_float_type(type)) {
        printf("Scalar-Float ");
    }
    
    printf("\n");
}

// Test conversion between type systems
void test_type_conversion() {
    printf("\nTesting Type Conversions\n");
    printf("======================\n");
    
    // Test fundamental type conversion
    ptx_type_t general_type = PTX_TYPE_F32;
    ptx_fundamental_type_t fund_type = ptx_type_to_fundamental_type(general_type);
    if (fund_type != (ptx_fundamental_type_t)-1) {
        printf("Successfully converted %s to fundamental type %d\n", 
               get_type_name(general_type), fund_type);
    } else {
        printf("Failed to convert %s to fundamental type\n", 
               get_type_name(general_type));
    }
    
    // Test alt float format conversion
    general_type = PTX_TYPE_BF16;
    ptx_alternate_float_format_t alt_type = ptx_type_to_alt_float_format(general_type);
    if (alt_type != (ptx_alternate_float_format_t)-1) {
        printf("Successfully converted %s to alt float type %d\n", 
               get_type_name(general_type), alt_type);
    } else {
        printf("Failed to convert %s to alt float type\n", 
               get_type_name(general_type));
    }
    
    // Test packed float type conversion
    general_type = PTX_TYPE_F16X2;
    ptx_packed_float_type_t packed_type = ptx_type_to_packed_float_type(general_type);
    if (packed_type != (ptx_packed_float_type_t)-1) {
        printf("Successfully converted %s to packed float type %d\n", 
               get_type_name(general_type), packed_type);
    } else {
        printf("Failed to convert %s to packed float type\n", 
               get_type_name(general_type));
    }
    
    // Test scalar float type conversion
    general_type = PTX_TYPE_F32;
    ptx_scalar_float_type_t scalar_type = ptx_type_to_scalar_float_type(general_type);
    if (scalar_type != (ptx_scalar_float_type_t)-1) {
        printf("Successfully converted %s to scalar float type %d\n", 
               get_type_name(general_type), scalar_type);
        
        // Test scalar type classification
        if (ptx_scalar_float_is_fundamental(scalar_type)) {
            printf("  Scalar type %s is a fundamental float type\n", 
                   get_type_name(general_type));
        } else {
            printf("  Scalar type %s is an alternate float format\n", 
                   get_type_name(general_type));
        }
    } else {
        printf("Failed to convert %s to scalar float type\n", 
               get_type_name(general_type));
    }
    
    // Test invalid conversion (integer type to alt float)
    general_type = PTX_TYPE_S32;
    alt_type = ptx_type_to_alt_float_format(general_type);
    if (alt_type != (ptx_alternate_float_format_t)-1) {
        printf("Unexpectedly converted %s to alt float type %d\n", 
               get_type_name(general_type), alt_type);
    } else {
        printf("Correctly failed to convert %s to alt float type\n", 
               get_type_name(general_type));
    }
    
    // Test invalid conversion (integer type to scalar float)
    scalar_type = ptx_type_to_scalar_float_type(general_type);
    if (scalar_type != (ptx_scalar_float_type_t)-1) {
        printf("Unexpectedly converted %s to scalar float type %d\n", 
               get_type_name(general_type), scalar_type);
    } else {
        printf("Correctly failed to convert %s to scalar float type\n", 
               get_type_name(general_type));
    }
}

int main() {
    printf("PTX Type Classification Test\n");
    printf("============================\n\n");
    
    // Test some integer types
    test_type_classification(PTX_TYPE_S32);
    test_type_classification(PTX_TYPE_U64);
    
    // Test fundamental float types
    test_type_classification(PTX_TYPE_F16);
    test_type_classification(PTX_TYPE_F32);
    test_type_classification(PTX_TYPE_F64);
    
    // Test bit types
    test_type_classification(PTX_TYPE_B32);
    test_type_classification(PTX_TYPE_B64);
    
    // Test predicate type
    test_type_classification(PTX_TYPE_PRED);
    
    // Test f16x2 (both fundamental and packed)
    test_type_classification(PTX_TYPE_F16X2);
    
    // Test alternate float formats
    test_type_classification(PTX_TYPE_BF16);
    test_type_classification(PTX_TYPE_E4M3);
    test_type_classification(PTX_TYPE_TF32);
    
    // Test non-fundamental packed types
    test_type_classification(PTX_TYPE_F32X2);
    test_type_classification(PTX_TYPE_BF16X2);
    test_type_classification(PTX_TYPE_E4M3X2);
    test_type_classification(PTX_TYPE_E4M3X4);
    
    // Test enum references
    printf("\nTesting Enum References\n");
    printf("======================\n");
    
    // Fundamental types - add explicit casts
    printf("PTX_FUNDAMENTAL_TYPE_S32 = %d = %s\n", 
           PTX_FUNDAMENTAL_TYPE_S32, get_type_name((ptx_type_t)PTX_FUNDAMENTAL_TYPE_S32));
    printf("PTX_FUNDAMENTAL_TYPE_F32 = %d = %s\n", 
           PTX_FUNDAMENTAL_TYPE_F32, get_type_name((ptx_type_t)PTX_FUNDAMENTAL_TYPE_F32));
    
    // Alt float formats - add explicit casts
    printf("PTX_ALT_FLOAT_BF16 = %d = %s\n", 
           PTX_ALT_FLOAT_BF16, get_type_name((ptx_type_t)PTX_ALT_FLOAT_BF16));
    printf("PTX_ALT_FLOAT_E4M3 = %d = %s\n", 
           PTX_ALT_FLOAT_E4M3, get_type_name((ptx_type_t)PTX_ALT_FLOAT_E4M3));
    
    // Packed float types - add explicit casts
    printf("PTX_PACKED_FLOAT_F16X2 = %d = %s\n", 
           PTX_PACKED_FLOAT_F16X2, get_type_name((ptx_type_t)PTX_PACKED_FLOAT_F16X2));
    printf("PTX_PACKED_FLOAT_BF16X2 = %d = %s\n", 
           PTX_PACKED_FLOAT_BF16X2, get_type_name((ptx_type_t)PTX_PACKED_FLOAT_BF16X2));
    
    // Scalar float types - add explicit casts
    printf("PTX_SCALAR_FLOAT_F32 = %d = %s\n", 
           PTX_SCALAR_FLOAT_F32, get_type_name((ptx_type_t)PTX_SCALAR_FLOAT_F32));
    printf("PTX_SCALAR_FLOAT_BF16 = %d = %s\n", 
           PTX_SCALAR_FLOAT_BF16, get_type_name((ptx_type_t)PTX_SCALAR_FLOAT_BF16));
    
    // Test type conversion functions
    test_type_conversion();
    
    return 0;
} 