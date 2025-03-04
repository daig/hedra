#include <ptx_parse/ptx_declaration_lhs_parse.h>
#include <ptx_print/ptx_declaration_lhs_print.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 256

/**
 * Helper function to cleanup a declaration LHS structure
 */
static void cleanup_decl_lhs(ptx_declaration_type_t *lhs) {
    if (lhs) {
        free(lhs->name);
        lhs->name = NULL;
    }
}

/**
 * Test parsing of declaration with managed attribute
 */
void test_managed_attribute_declaration() {
    printf("Testing declaration with managed attribute...\n");
    
    const char* test_str = ".attribute .managed .global .u32 g";
    ptx_declaration_type_t lhs = {0};
    
    printf("About to parse: \"%s\"\n", test_str);
    bool result = parse_declaration_lhs(test_str, &lhs);
    printf("Parsing \"%s\": %s\n", test_str, result ? "SUCCESS" : "FAILED");
    
    if (result) {
        assert(lhs.has_attribute);
        assert(lhs.attribute.tag == PTX_ATTR_MANAGED);
        assert(lhs.statespace == PTX_STATE_GLOBAL);
        assert(lhs.type == PTX_TYPE_U32);
        assert(strcmp(lhs.name, "g") == 0);
        
        // Test printing
        char buffer[BUFFER_SIZE] = {0};
        int print_result = print_declaration_lhs_to_buffer(buffer, BUFFER_SIZE, &lhs);
        printf("Printed: \"%s\"\n", buffer);
        
        assert(print_result > 0);
        assert(strstr(buffer, ".attribute .managed") != NULL);
        assert(strstr(buffer, ".global") != NULL);
        assert(strstr(buffer, ".u32") != NULL);
        assert(strstr(buffer, "g") != NULL);
        
        cleanup_decl_lhs(&lhs);
        printf("Declaration with managed attribute test passed!\n\n");
    } else {
        printf("Declaration with managed attribute test failed!\n\n");
        assert(0 && "Declaration with managed attribute parsing should not fail");
    }
}

/**
 * Test parsing of declaration with unified attribute
 */
void test_unified_attribute_declaration() {
    printf("Testing declaration with unified attribute...\n");
    
    const char* test_str = ".attribute .unified(0xAB, 0xCD) .global .f32 f";
    ptx_declaration_type_t lhs = {0};
    
    printf("About to parse: \"%s\"\n", test_str);
    bool result = parse_declaration_lhs(test_str, &lhs);
    printf("Parsing \"%s\": %s\n", test_str, result ? "SUCCESS" : "FAILED");
    
    if (result) {
        assert(lhs.has_attribute);
        assert(lhs.attribute.tag == PTX_ATTR_UNIFIED);
        assert(lhs.attribute.unified.uuid1 == 0xAB);
        assert(lhs.attribute.unified.uuid2 == 0xCD);
        assert(lhs.statespace == PTX_STATE_GLOBAL);
        assert(lhs.type == PTX_TYPE_F32);
        assert(strcmp(lhs.name, "f") == 0);
        
        // Test printing
        char buffer[BUFFER_SIZE] = {0};
        int print_result = print_declaration_lhs_to_buffer(buffer, BUFFER_SIZE, &lhs);
        printf("Printed: \"%s\"\n", buffer);
        
        assert(print_result > 0);
        assert(strstr(buffer, ".attribute .unified") != NULL);
        assert(strstr(buffer, ".global") != NULL);
        assert(strstr(buffer, ".f32") != NULL);
        assert(strstr(buffer, "f") != NULL);
        
        cleanup_decl_lhs(&lhs);
        printf("Declaration with unified attribute test passed!\n\n");
    } else {
        printf("Declaration with unified attribute test failed!\n\n");
        assert(0 && "Declaration with unified attribute parsing should not fail");
    }
}

int main() {
    printf("Running PTX declaration with attribute tests\n");
    printf("===========================================\n\n");
    
    test_managed_attribute_declaration();
    test_unified_attribute_declaration();
    
    printf("All tests passed!\n");
    return 0;
} 