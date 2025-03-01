#include <ptx_print/ptx_directive_print.h>
#include <ptx_parse/ptx_directive_parse.h>
#include <ptx_ast/ptx_directive.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define BUFFER_SIZE 128

// Function to test round-trip parsing and printing
void test_parse_print_roundtrip(const char* directive_str) {
    ptx_directive_t directive;
    char buffer[BUFFER_SIZE] = {0};
    
    // Parse the directive string
    assert(parse_directive(directive_str, &directive) == true);
    
    // Print the directive to the buffer
    int printed = print_directive_to_buffer(buffer, BUFFER_SIZE, directive);
    assert(printed > 0);
    
    // Check that the printed string matches the original
    // Note: the original string does not have the leading dot, but printed does
    char expected[BUFFER_SIZE] = {0};
    snprintf(expected, BUFFER_SIZE, ".%s", directive_str);
    assert(strcmp(buffer, expected) == 0);
    
    printf("Roundtrip test passed for directive: '%s' -> '%s'\n", directive_str, buffer);
}

void test_basic_directives() {
    printf("Testing basic directive printing...\n");
    
    // Test printing to buffer
    char buffer[BUFFER_SIZE];
    
    // Test .version directive
    ptx_directive_t version_directive = PTX_DIRECTIVE_VERSION;
    int result = print_directive_to_buffer(buffer, BUFFER_SIZE, version_directive);
    assert(result > 0);
    assert(strcmp(buffer, ".version") == 0);
    
    // Test .target directive
    ptx_directive_t target_directive = PTX_DIRECTIVE_TARGET;
    result = print_directive_to_buffer(buffer, BUFFER_SIZE, target_directive);
    assert(result > 0);
    assert(strcmp(buffer, ".target") == 0);
    
    // Test .entry directive
    ptx_directive_t entry_directive = PTX_DIRECTIVE_ENTRY;
    result = print_directive_to_buffer(buffer, BUFFER_SIZE, entry_directive);
    assert(result > 0);
    assert(strcmp(buffer, ".entry") == 0);
    
    printf("Basic directive printing tests passed!\n");
}

void test_roundtrip_parsing_printing() {
    printf("Testing roundtrip parsing and printing...\n");
    
    // Test all directives
    test_parse_print_roundtrip("version");
    test_parse_print_roundtrip("target");
    test_parse_print_roundtrip("entry");
    test_parse_print_roundtrip("func");
    test_parse_print_roundtrip("global");
    test_parse_print_roundtrip("local");
    test_parse_print_roundtrip("const");
    test_parse_print_roundtrip("shared");
    test_parse_print_roundtrip("param");
    test_parse_print_roundtrip("reg");
    test_parse_print_roundtrip("address_size");
    test_parse_print_roundtrip("maxnreg");
    test_parse_print_roundtrip("maxntid");
    
    printf("Roundtrip parsing and printing tests passed!\n");
}

void test_file_output() {
    printf("Testing directive printing to file...\n");
    
    // Open a temporary file for testing
    FILE* temp_file = tmpfile();
    assert(temp_file != NULL);
    
    // Print directives to the file
    assert(print_directive_to_file(temp_file, PTX_DIRECTIVE_VERSION) == true);
    assert(print_directive_to_file(temp_file, PTX_DIRECTIVE_TARGET) == true);
    assert(print_directive_to_file(temp_file, PTX_DIRECTIVE_FUNC) == true);
    
    // Rewind the file to read from the beginning
    rewind(temp_file);
    
    // Read and check the contents
    char buffer[BUFFER_SIZE];
    assert(fgets(buffer, BUFFER_SIZE, temp_file) != NULL);
    assert(strcmp(buffer, ".version.target.func") == 0);
    
    // Close the file
    fclose(temp_file);
    
    printf("File output tests passed!\n");
}

int main() {
    printf("=== PTX Directive Printer Tests ===\n");
    
    test_basic_directives();
    test_roundtrip_parsing_printing();
    test_file_output();
    
    printf("All directive printer tests passed!\n");
    return 0;
} 