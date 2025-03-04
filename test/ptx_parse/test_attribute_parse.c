#include <ptx_parse/ptx_attribute_parse.h>
#include <ptx_print/ptx_attribute_print.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 256

/**
 * Test parsing of managed attribute
 */
void test_managed_attribute() {
    printf("Testing managed attribute parsing...\n");
    
    const char* test_str = ".attribute(.managed)";
    ptx_attribute_t attr = {0};
    const char* end = NULL;
    
    printf("About to parse: \"%s\"\n", test_str);
    bool result = parse_attribute(test_str, &attr, &end);
    printf("Parsing \"%s\": %s\n", test_str, result ? "SUCCESS" : "FAILED");
    
    if (result) {
        assert(attr.tag == PTX_ATTR_MANAGED);
        printf("Managed attribute parsing test passed!\n\n");
    } else {
        printf("Managed attribute parsing test failed!\n\n");
        assert(0 && "Managed attribute parsing should not fail");
    }
    
    // Test printing
    char buffer[BUFFER_SIZE] = {0};
    int print_result = print_attribute_to_buffer(buffer, BUFFER_SIZE, &attr);
    assert(print_result > 0);
    
    printf("Expected: \".attribute(.managed)\"\n");
    printf("Got:      \"%s\"\n", buffer);
    assert(strcmp(buffer, ".attribute(.managed)") == 0);
}

/**
 * Test parsing of unified attribute with parameters
 */
void test_unified_attribute() {
    printf("Testing unified attribute parsing...\n");
    
    const char* test_str = ".attribute(.unified(19, 95))";
    ptx_attribute_t attr = {0};
    const char* end = NULL;
    
    printf("About to parse: \"%s\"\n", test_str);
    bool result = parse_attribute(test_str, &attr, &end);
    printf("Parsing \"%s\": %s\n", test_str, result ? "SUCCESS" : "FAILED");
    
    if (result) {
        assert(attr.tag == PTX_ATTR_UNIFIED);
        assert(attr.unified.uuid1 == 19);
        assert(attr.unified.uuid2 == 95);
        printf("Unified attribute parsing test passed!\n\n");
    } else {
        printf("Unified attribute parsing test failed!\n\n");
        assert(0 && "Unified attribute parsing should not fail");
    }
    
    // Test printing
    char buffer[BUFFER_SIZE] = {0};
    int print_result = print_attribute_to_buffer(buffer, BUFFER_SIZE, &attr);
    assert(print_result > 0);
    
    printf("Expected: \".attribute(.unified(19, 95))\"\n");
    printf("Got:      \"%s\"\n", buffer);
    assert(strcmp(buffer, ".attribute(.unified(19, 95))") == 0);
}

/**
 * Test parsing of unified attribute with hex parameters
 */
void test_unified_attribute_hex() {
    printf("Testing unified attribute parsing with hex values...\n");
    
    const char* test_str = ".attribute(.unified(0xAB, 0xCD))";
    ptx_attribute_t attr = {0};
    const char* end = NULL;
    
    printf("About to parse: \"%s\"\n", test_str);
    bool result = parse_attribute(test_str, &attr, &end);
    printf("Parsing \"%s\": %s\n", test_str, result ? "SUCCESS" : "FAILED");
    
    if (result) {
        assert(attr.tag == PTX_ATTR_UNIFIED);
        assert(attr.unified.uuid1 == 0xAB);
        assert(attr.unified.uuid2 == 0xCD);
        printf("Unified attribute (hex) parsing test passed!\n\n");
    } else {
        printf("Unified attribute (hex) parsing test failed!\n\n");
        assert(0 && "Unified attribute (hex) parsing should not fail");
    }
}

int main() {
    printf("Running PTX attribute parser tests\n");
    printf("=================================\n\n");
    
    test_managed_attribute();
    test_unified_attribute();
    test_unified_attribute_hex();
    
    printf("All tests passed!\n");
    return 0;
} 