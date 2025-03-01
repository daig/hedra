#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <ptx_ast/>ptx_type.h>
#include <ptx_ast/>ptx_type_graphics.h>

// Helper function to print the result of a boolean test
void print_test_result(const char* test_name, bool result) {
    printf("%-40s: %s\n", test_name, result ? "PASS" : "FAIL");
}

// Print the name of a PTX type kind
const char* get_type_kind_name(ptx_type_kind_t kind) {
    switch (kind) {
        case PTX_TYPE_KIND_SIGNED_INT: return "SIGNED_INT";
        case PTX_TYPE_KIND_UNSIGNED_INT: return "UNSIGNED_INT";
        case PTX_TYPE_KIND_FLOAT: return "FLOAT";
        case PTX_TYPE_KIND_BITS: return "BITS";
        case PTX_TYPE_KIND_PRED: return "PRED";
        case PTX_TYPE_KIND_GRAPHICS: return "GRAPHICS";
        default: return "UNKNOWN";
    }
}

int main() {
    printf("Testing PTX Graphics Types\n");
    printf("==========================\n\n");

    // Test 1: Verify ptx_is_graphics_type function
    printf("Test Set 1: ptx_is_graphics_type\n");
    bool test1_1 = ptx_is_graphics_type(PTX_TYPE_TEXREF);
    bool test1_2 = ptx_is_graphics_type(PTX_TYPE_SAMPLERREF);
    bool test1_3 = ptx_is_graphics_type(PTX_TYPE_SURFREF);
    bool test1_4 = !ptx_is_graphics_type(PTX_TYPE_F32); // Non-graphics type
    bool test1_5 = !ptx_is_graphics_type(PTX_TYPE_S64); // Non-graphics type
    
    print_test_result("PTX_TYPE_TEXREF is graphics type", test1_1);
    print_test_result("PTX_TYPE_SAMPLERREF is graphics type", test1_2);
    print_test_result("PTX_TYPE_SURFREF is graphics type", test1_3);
    print_test_result("PTX_TYPE_F32 is not graphics type", test1_4);
    print_test_result("PTX_TYPE_S64 is not graphics type", test1_5);
    printf("\n");

    // Test 2: Verify specific graphics type functions
    printf("Test Set 2: Specific Graphics Type Functions\n");
    bool test2_1 = ptx_is_texture_ref(PTX_TYPE_TEXREF);
    bool test2_2 = !ptx_is_texture_ref(PTX_TYPE_SAMPLERREF);
    bool test2_3 = ptx_is_sampler_ref(PTX_TYPE_SAMPLERREF);
    bool test2_4 = !ptx_is_sampler_ref(PTX_TYPE_TEXREF);
    bool test2_5 = ptx_is_surface_ref(PTX_TYPE_SURFREF);
    bool test2_6 = !ptx_is_surface_ref(PTX_TYPE_TEXREF);
    
    print_test_result("PTX_TYPE_TEXREF is texture ref", test2_1);
    print_test_result("PTX_TYPE_SAMPLERREF is not texture ref", test2_2);
    print_test_result("PTX_TYPE_SAMPLERREF is sampler ref", test2_3);
    print_test_result("PTX_TYPE_TEXREF is not sampler ref", test2_4);
    print_test_result("PTX_TYPE_SURFREF is surface ref", test2_5);
    print_test_result("PTX_TYPE_TEXREF is not surface ref", test2_6);
    printf("\n");

    // Test 3: Verify ptx_type_to_graphics_type function
    printf("Test Set 3: ptx_type_to_graphics_type\n");
    ptx_graphics_type_t graphics_type1 = ptx_type_to_graphics_type(PTX_TYPE_TEXREF);
    ptx_graphics_type_t graphics_type2 = ptx_type_to_graphics_type(PTX_TYPE_SAMPLERREF);
    ptx_graphics_type_t graphics_type3 = ptx_type_to_graphics_type(PTX_TYPE_SURFREF);
    ptx_graphics_type_t graphics_type4 = ptx_type_to_graphics_type(PTX_TYPE_F32); // Should return -1
    
    bool test3_1 = graphics_type1 == PTX_GRAPHICS_TYPE_TEXREF;
    bool test3_2 = graphics_type2 == PTX_GRAPHICS_TYPE_SAMPLERREF;
    bool test3_3 = graphics_type3 == PTX_GRAPHICS_TYPE_SURFREF;
    bool test3_4 = graphics_type4 == (ptx_graphics_type_t)-1;
    
    print_test_result("PTX_TYPE_TEXREF maps to PTX_GRAPHICS_TYPE_TEXREF", test3_1);
    print_test_result("PTX_TYPE_SAMPLERREF maps to PTX_GRAPHICS_TYPE_SAMPLERREF", test3_2);
    print_test_result("PTX_TYPE_SURFREF maps to PTX_GRAPHICS_TYPE_SURFREF", test3_3);
    print_test_result("PTX_TYPE_F32 returns -1", test3_4);
    printf("\n");

    // Test 4: Verify ptx_get_type_kind function for graphics types
    printf("Test Set 4: ptx_get_type_kind for Graphics Types\n");
    ptx_type_kind_t kind1 = ptx_get_type_kind(PTX_TYPE_TEXREF);
    ptx_type_kind_t kind2 = ptx_get_type_kind(PTX_TYPE_SAMPLERREF);
    ptx_type_kind_t kind3 = ptx_get_type_kind(PTX_TYPE_SURFREF);
    
    printf("PTX_TYPE_TEXREF kind: %s\n", get_type_kind_name(kind1));
    printf("PTX_TYPE_SAMPLERREF kind: %s\n", get_type_kind_name(kind2));
    printf("PTX_TYPE_SURFREF kind: %s\n", get_type_kind_name(kind3));
    
    bool test4_1 = kind1 == PTX_TYPE_KIND_GRAPHICS;
    bool test4_2 = kind2 == PTX_TYPE_KIND_GRAPHICS;
    bool test4_3 = kind3 == PTX_TYPE_KIND_GRAPHICS;
    
    print_test_result("PTX_TYPE_TEXREF has kind GRAPHICS", test4_1);
    print_test_result("PTX_TYPE_SAMPLERREF has kind GRAPHICS", test4_2);
    print_test_result("PTX_TYPE_SURFREF has kind GRAPHICS", test4_3);
    printf("\n");

    // Summary
    printf("Summary\n");
    printf("=======\n");
    int total_tests = 5 + 6 + 4 + 3;
    int passed_tests = 
        (test1_1 + test1_2 + test1_3 + test1_4 + test1_5) +
        (test2_1 + test2_2 + test2_3 + test2_4 + test2_5 + test2_6) +
        (test3_1 + test3_2 + test3_3 + test3_4) +
        (test4_1 + test4_2 + test4_3);
    
    printf("Tests passed: %d/%d\n", passed_tests, total_tests);
    
    return (passed_tests == total_tests) ? 0 : 1;
} 