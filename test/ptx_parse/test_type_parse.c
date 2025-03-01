#include <ptx_parse/ptx_type_parse.h>
#include <stdio.h>
#include <assert.h>

void test_fundamental_types() {
    printf("Testing fundamental type parsing...\n");
    
    ptx_type_t type;
    
    // Signed integer types
    assert(parse_type(".s8", &type) == true);
    assert(type == PTX_TYPE_S8);
    
    assert(parse_type(".s16", &type) == true);
    assert(type == PTX_TYPE_S16);
    
    assert(parse_type(".s32", &type) == true);
    assert(type == PTX_TYPE_S32);
    
    assert(parse_type(".s64", &type) == true);
    assert(type == PTX_TYPE_S64);
    
    // Unsigned integer types
    assert(parse_type(".u8", &type) == true);
    assert(type == PTX_TYPE_U8);
    
    assert(parse_type(".u16", &type) == true);
    assert(type == PTX_TYPE_U16);
    
    assert(parse_type(".u32", &type) == true);
    assert(type == PTX_TYPE_U32);
    
    assert(parse_type(".u64", &type) == true);
    assert(type == PTX_TYPE_U64);
    
    // Floating-point types
    assert(parse_type(".f16", &type) == true);
    assert(type == PTX_TYPE_F16);
    
    assert(parse_type(".f32", &type) == true);
    assert(type == PTX_TYPE_F32);
    
    assert(parse_type(".f64", &type) == true);
    assert(type == PTX_TYPE_F64);
    
    // Bit types
    assert(parse_type(".b8", &type) == true);
    assert(type == PTX_TYPE_B8);
    
    assert(parse_type(".b16", &type) == true);
    assert(type == PTX_TYPE_B16);
    
    assert(parse_type(".b32", &type) == true);
    assert(type == PTX_TYPE_B32);
    
    assert(parse_type(".b64", &type) == true);
    assert(type == PTX_TYPE_B64);
    
    assert(parse_type(".b128", &type) == true);
    assert(type == PTX_TYPE_B128);
    
    // Predicate type
    assert(parse_type(".pred", &type) == true);
    assert(type == PTX_TYPE_PRED);
    
    // Packed floating-point type
    assert(parse_type(".f16x2", &type) == true);
    assert(type == PTX_TYPE_F16X2);
    
    printf("Fundamental type parsing tests passed!\n");
}

void test_alternate_float_formats() {
    printf("Testing alternate float format parsing...\n");
    
    ptx_type_t type;
    
    assert(parse_type(".bf16", &type) == true);
    assert(type == PTX_TYPE_BF16);
    
    assert(parse_type(".e4m3", &type) == true);
    assert(type == PTX_TYPE_E4M3);
    
    assert(parse_type(".e5m2", &type) == true);
    assert(type == PTX_TYPE_E5M2);
    
    assert(parse_type(".tf32", &type) == true);
    assert(type == PTX_TYPE_TF32);
    
    assert(parse_type(".ue4m3", &type) == true);
    assert(type == PTX_TYPE_UE4M3);
    
    assert(parse_type(".e2m1", &type) == true);
    assert(type == PTX_TYPE_E2M1);
    
    assert(parse_type(".e2m3", &type) == true);
    assert(type == PTX_TYPE_E2M3);
    
    assert(parse_type(".e3m2", &type) == true);
    assert(type == PTX_TYPE_E3M2);
    
    assert(parse_type(".ue8m0", &type) == true);
    assert(type == PTX_TYPE_UE8M0);
    
    printf("Alternate float format parsing tests passed!\n");
}

void test_packed_types() {
    printf("Testing packed type parsing...\n");
    
    ptx_type_t type;
    
    // Packed float types
    assert(parse_type(".f32x2", &type) == true);
    assert(type == PTX_TYPE_F32X2);
    
    assert(parse_type(".bf16x2", &type) == true);
    assert(type == PTX_TYPE_BF16X2);
    
    // Sample a few of the many packed format types
    assert(parse_type(".e4m3x2", &type) == true);
    assert(type == PTX_TYPE_E4M3X2);
    
    assert(parse_type(".e5m2x4", &type) == true);
    assert(type == PTX_TYPE_E5M2X4);
    
    // Packed integer types
    assert(parse_type(".u16x2", &type) == true);
    assert(type == PTX_TYPE_U16X2);
    
    assert(parse_type(".s16x2", &type) == true);
    assert(type == PTX_TYPE_S16X2);
    
    printf("Packed type parsing tests passed!\n");
}

void test_graphics_types() {
    printf("Testing graphics type parsing...\n");
    
    ptx_type_t type;
    
    assert(parse_type(".texref", &type) == true);
    assert(type == PTX_TYPE_TEXREF);
    
    assert(parse_type(".samplerref", &type) == true);
    assert(type == PTX_TYPE_SAMPLERREF);
    
    assert(parse_type(".surfref", &type) == true);
    assert(type == PTX_TYPE_SURFREF);
    
    printf("Graphics type parsing tests passed!\n");
}

void test_vector_types() {
    printf("Testing vector type parsing...\n");
    
    ptx_type_t type;
    
    // PTX vector types are a bit specialized
    // For now we're only implementing .v2.f32, which maps to f32x2
    assert(parse_type(".v2.f32", &type) == true);
    assert(type == PTX_TYPE_F32X2);
    
    printf("Vector type parsing tests passed!\n");
}

void test_case_insensitivity() {
    printf("Testing case insensitivity...\n");
    
    ptx_type_t type;
    
    assert(parse_type(".S32", &type) == true);
    assert(type == PTX_TYPE_S32);
    
    assert(parse_type(".f32", &type) == true);
    assert(type == PTX_TYPE_F32);
    
    assert(parse_type(".PRED", &type) == true);
    assert(type == PTX_TYPE_PRED);
    
    assert(parse_type(".TeXrEf", &type) == true);
    assert(type == PTX_TYPE_TEXREF);
    
    printf("Case insensitivity tests passed!\n");
}

void test_invalid_types() {
    printf("Testing invalid type parsing...\n");
    
    ptx_type_t type;
    
    // Missing dot
    assert(parse_type("s32", &type) == false);
    
    // Invalid type
    assert(parse_type(".xyz", &type) == false);
    
    // Misspelled type
    assert(parse_type(".s33", &type) == false);
    
    // Whitespace
    assert(parse_type(" .s32", &type) == false);
    
    // Empty string
    assert(parse_type("", &type) == false);
    
    // NULL
    assert(parse_type(NULL, &type) == false);
    
    printf("Invalid type parsing tests passed!\n");
}

int main() {
    printf("Running PTX type parsing tests...\n");
    
    test_fundamental_types();
    test_alternate_float_formats();
    test_packed_types();
    test_graphics_types();
    test_vector_types();
    test_case_insensitivity();
    test_invalid_types();
    
    printf("All type parsing tests passed!\n");
    return 0;
} 