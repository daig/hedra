#include <ptx_print/ptx_type_print.h>
#include <ptx_parse/ptx_type_parse.h>
#include <ptx_ast/ptx_type.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUFFER_SIZE 128

// Helper function to normalize whitespace for comparison
void normalize_whitespace(char* str) {
    if (!str) return;
    
    char* src = str;
    char* dst = str;
    
    // Skip leading whitespace
    while (*src && (*src == ' ' || *src == '\t' || *src == '\n')) {
        src++;
    }
    
    // Copy non-whitespace characters and collapse multiple whitespace to single space
    int was_whitespace = 0;
    while (*src) {
        if (*src == ' ' || *src == '\t' || *src == '\n') {
            if (!was_whitespace) {
                *dst++ = ' ';
                was_whitespace = 1;
            }
        } else {
            *dst++ = *src;
            was_whitespace = 0;
        }
        src++;
    }
    
    // Remove trailing whitespace
    if (dst > str && was_whitespace) {
        dst--;
    }
    
    // Null-terminate the string
    *dst = '\0';
}

// Test parse -> print -> compare roundtrip
void test_parse_print_roundtrip(const char* type_str) {
    // Parse type from string
    ptx_type_t type;
    bool parse_result = parse_type(type_str, &type);
    assert(parse_result && "Failed to parse type");
    
    // Print the type to a buffer
    char buffer[BUFFER_SIZE];
    int chars_written = print_type_to_buffer(buffer, BUFFER_SIZE, type);
    assert(chars_written > 0 && "Failed to print type to buffer");
    buffer[chars_written] = '\0';  // Ensure null termination
    
    // Normalize whitespace in both strings for comparison
    char original[BUFFER_SIZE];
    strncpy(original, type_str, BUFFER_SIZE - 1);
    original[BUFFER_SIZE - 1] = '\0';
    normalize_whitespace(original);
    normalize_whitespace(buffer);
    
    // Compare the strings
    printf("Original: '%s', Printed: '%s'\n", original, buffer);
    assert(strcmp(original, buffer) == 0 && "Printed type doesn't match original");
}

// Test basic types
void test_basic_types() {
    // Integer types
    test_parse_print_roundtrip(".s8");
    test_parse_print_roundtrip(".s16");
    test_parse_print_roundtrip(".s32");
    test_parse_print_roundtrip(".s64");
    test_parse_print_roundtrip(".u8");
    test_parse_print_roundtrip(".u16");
    test_parse_print_roundtrip(".u32");
    test_parse_print_roundtrip(".u64");
    
    // Floating-point types
    test_parse_print_roundtrip(".f16");
    test_parse_print_roundtrip(".f32");
    test_parse_print_roundtrip(".f64");
    
    // Bit types
    test_parse_print_roundtrip(".b8");
    test_parse_print_roundtrip(".b16");
    test_parse_print_roundtrip(".b32");
    test_parse_print_roundtrip(".b64");
    test_parse_print_roundtrip(".b128");
    
    // Predicate type
    test_parse_print_roundtrip(".pred");
}

// Test packed and alternate format types
void test_packed_and_alternate_types() {
    // Packed fundamental floating-point type
    test_parse_print_roundtrip(".f16x2");
    
    // Alternate non-packed floating-point formats
    test_parse_print_roundtrip(".bf16");
    test_parse_print_roundtrip(".e4m3");
    test_parse_print_roundtrip(".e5m2");
    test_parse_print_roundtrip(".tf32");
    test_parse_print_roundtrip(".ue4m3");
    test_parse_print_roundtrip(".e2m1");
    test_parse_print_roundtrip(".e2m3");
    test_parse_print_roundtrip(".e3m2");
    test_parse_print_roundtrip(".ue8m0");
    
    // Non-fundamental packed floating-point types
    test_parse_print_roundtrip(".f32x2");
    test_parse_print_roundtrip(".bf16x2");
    test_parse_print_roundtrip(".e4m3x2");
    test_parse_print_roundtrip(".e5m2x2");
    test_parse_print_roundtrip(".e2m3x2");
    test_parse_print_roundtrip(".e3m2x2");
    test_parse_print_roundtrip(".ue8m0x2");
    test_parse_print_roundtrip(".e2m1x2");
    test_parse_print_roundtrip(".e4m3x4");
    test_parse_print_roundtrip(".e5m2x4");
    test_parse_print_roundtrip(".e2m3x4");
    test_parse_print_roundtrip(".e3m2x4");
    test_parse_print_roundtrip(".e2m1x4");
    
    // Packed integer types
    test_parse_print_roundtrip(".u16x2");
    test_parse_print_roundtrip(".s16x2");
}

// Test graphics types
void test_graphics_types() {
    test_parse_print_roundtrip(".texref");
    test_parse_print_roundtrip(".samplerref");
    test_parse_print_roundtrip(".surfref");
}

// Test file output
void test_file_output() {
    // Create a temporary file
    FILE* file = tmpfile();
    assert(file != NULL && "Failed to create temporary file");
    
    // Print some types to the file
    ptx_type_t type = PTX_TYPE_S32;
    bool success = print_type_to_file(file, type);
    assert(success && "Failed to print type to file");
    
    type = PTX_TYPE_F32;
    success = print_type_to_file(file, type);
    assert(success && "Failed to print type to file");
    
    type = PTX_TYPE_PRED;
    success = print_type_to_file(file, type);
    assert(success && "Failed to print type to file");
    
    // Rewind the file to read its content
    rewind(file);
    
    // Read the file content
    char buffer[BUFFER_SIZE];
    size_t bytes_read = fread(buffer, 1, BUFFER_SIZE - 1, file);
    buffer[bytes_read] = '\0';
    
    // Close the file
    fclose(file);
    
    // Verify the content
    assert(strstr(buffer, ".s32") != NULL && "File content missing .s32");
    assert(strstr(buffer, ".f32") != NULL && "File content missing .f32");
    assert(strstr(buffer, ".pred") != NULL && "File content missing .pred");
}

// Main function
int main() {
    printf("Testing PTX type printing...\n");
    
    test_basic_types();
    test_packed_and_alternate_types();
    test_graphics_types();
    test_file_output();
    
    printf("All tests passed successfully!\n");
    return 0;
} 