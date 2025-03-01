#include <ptx_print/ptx_identifier_print.h>
#include <ptx_parse/ptx_identifier_parse.h>
#include <ptx_ast/ptx_identifier.h>
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
    if (dst > str && *(dst-1) == ' ') {
        dst--;
    }
    
    *dst = '\0';
}

// Function to test round-trip parsing and printing
void test_parse_print_roundtrip(const char* identifier_str) {
    ptx_identifier_t* identifier = NULL;
    char buffer[BUFFER_SIZE] = {0};
    
    // Parse the identifier string
    assert(parse_identifier(identifier_str, &identifier) == true);
    assert(identifier != NULL);
    
    // Print the identifier to the buffer
    int printed = print_identifier_to_buffer(buffer, BUFFER_SIZE, *identifier);
    assert(printed > 0);
    
    // Normalize whitespace for comparison
    char normalized_input[BUFFER_SIZE];
    char normalized_output[BUFFER_SIZE];
    
    strncpy(normalized_input, identifier_str, BUFFER_SIZE);
    strncpy(normalized_output, buffer, BUFFER_SIZE);
    
    normalize_whitespace(normalized_input);
    normalize_whitespace(normalized_output);
    
    // Check that the printed string matches the original
    printf("Original: '%s', Printed: '%s'\n", normalized_input, normalized_output);
    assert(strcmp(normalized_input, normalized_output) == 0);
    
    // Free the allocated memory
    if (identifier->tag == PTX_IDENTIFIER_USER_DEFINED && identifier->user_defined) {
        free(identifier->user_defined);
    }
    free(identifier);
    
    printf("Roundtrip test passed for identifier: '%s'\n", identifier_str);
}

void test_basic_identifiers() {
    printf("Testing basic identifier printing...\n");
    
    // Test printing to buffer
    char buffer[BUFFER_SIZE];
    
    // Test predefined identifier
    ptx_identifier_t predefined_id = {
        .tag = PTX_IDENTIFIER_PREDEFINED,
        .predefined = TID
    };
    int result = print_identifier_to_buffer(buffer, BUFFER_SIZE, predefined_id);
    assert(result > 0);
    assert(strcmp(buffer, "%tid") == 0);
    
    // Test user-defined identifier
    char* user_defined_name = strdup("my_var");
    ptx_identifier_t user_defined_id = {
        .tag = PTX_IDENTIFIER_USER_DEFINED,
        .user_defined = user_defined_name
    };
    result = print_identifier_to_buffer(buffer, BUFFER_SIZE, user_defined_id);
    assert(result > 0);
    assert(strcmp(buffer, "my_var") == 0);
    free(user_defined_name);
    
    printf("Basic identifier printing tests passed!\n");
}

void test_roundtrip_parsing_printing() {
    printf("Testing roundtrip parsing and printing...\n");
    
    // Test predefined identifiers
    test_parse_print_roundtrip("%clock");
    test_parse_print_roundtrip("%tid");
    test_parse_print_roundtrip("%laneid");
    test_parse_print_roundtrip("%nctaid");
    
    // Test user-defined identifiers
    test_parse_print_roundtrip("variable");
    test_parse_print_roundtrip("_var");
    test_parse_print_roundtrip("$var");
    test_parse_print_roundtrip("var_123");
    
    printf("Roundtrip parsing and printing tests passed!\n");
}

void test_file_output() {
    printf("Testing identifier printing to file...\n");
    
    // Open a temporary file for testing
    FILE* temp_file = tmpfile();
    assert(temp_file != NULL);
    
    // Create some identifiers
    ptx_identifier_t predefined_id = {
        .tag = PTX_IDENTIFIER_PREDEFINED,
        .predefined = CLOCK
    };
    
    char* user_defined_name = strdup("my_variable");
    ptx_identifier_t user_defined_id = {
        .tag = PTX_IDENTIFIER_USER_DEFINED,
        .user_defined = user_defined_name
    };
    
    // Print identifiers to the file
    assert(print_identifier_to_file(temp_file, predefined_id) == true);
    assert(print_identifier_to_file(temp_file, user_defined_id) == true);
    
    // Reset file position to the beginning
    rewind(temp_file);
    
    // Read and verify the content
    char file_content[BUFFER_SIZE * 2] = {0};
    size_t bytes_read = fread(file_content, 1, sizeof(file_content) - 1, temp_file);
    file_content[bytes_read] = '\0';
    
    printf("File content: '%s'\n", file_content);
    assert(strstr(file_content, "%clock") != NULL);
    assert(strstr(file_content, "my_variable") != NULL);
    
    // Clean up
    fclose(temp_file);
    free(user_defined_name);
    
    printf("File output tests passed!\n");
}

int main() {
    printf("Running identifier printer tests...\n");
    
    test_basic_identifiers();
    test_roundtrip_parsing_printing();
    test_file_output();
    
    printf("All identifier printer tests passed!\n");
    return 0;
} 