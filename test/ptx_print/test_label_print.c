#include <ptx_print/ptx_label_print.h>
#include <ptx_parse/ptx_label_parse.h>
#include <ptx_ast/ptx_label.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 128

// Function to test round-trip parsing and printing
void test_parse_print_roundtrip(const char* label_str) {
    ptx_label_t* label = NULL;
    char buffer[BUFFER_SIZE] = {0};
    
    // Parse the label string
    assert(parse_label(label_str, &label) == true);
    assert(label != NULL);
    
    // Print the label to the buffer
    int printed = print_label_to_buffer(buffer, BUFFER_SIZE, label);
    assert(printed > 0);
    
    // Check that the printed string matches the original (modulo whitespace)
    // Need to handle potential whitespace in original before the colon
    char expected[BUFFER_SIZE] = {0};
    const char* colon_pos = strchr(label_str, ':');
    assert(colon_pos != NULL);
    
    // Calculate the length of the identifier (strip whitespace before colon)
    const char* id_end = colon_pos;
    while (id_end > label_str && isspace(*(id_end-1))) {
        id_end--;
    }
    
    // Calculate the start of the identifier (skip leading whitespace)
    const char* id_start = label_str;
    while (isspace(*id_start)) {
        id_start++;
    }
    
    // Copy the cleaned identifier and add a colon
    size_t id_len = id_end - id_start;
    strncpy(expected, id_start, id_len);
    expected[id_len] = ':';
    expected[id_len + 1] = '\0';
    
    printf("Original: '%s', Expected: '%s', Got: '%s'\n", label_str, expected, buffer);
    assert(strcmp(buffer, expected) == 0);
    
    // Clean up
    free(label->name);
    free(label);
    
    printf("Roundtrip test passed for label: '%s' -> '%s'\n", label_str, buffer);
}

void test_basic_label_printing() {
    printf("Testing basic label printing...\n");
    
    // Test printing to buffer
    char buffer[BUFFER_SIZE];
    
    // Create a test label
    ptx_label_t test_label;
    test_label.name = strdup("test_label");
    
    // Test printing to buffer
    int result = print_label_to_buffer(buffer, BUFFER_SIZE, &test_label);
    assert(result > 0);
    assert(strcmp(buffer, "test_label:") == 0);
    
    // Create another test label
    ptx_label_t special_label;
    special_label.name = strdup("$special");
    
    // Test printing to buffer
    result = print_label_to_buffer(buffer, BUFFER_SIZE, &special_label);
    assert(result > 0);
    assert(strcmp(buffer, "$special:") == 0);
    
    // Clean up
    free(test_label.name);
    free(special_label.name);
    
    printf("Basic label printing tests passed!\n");
}

void test_roundtrip_parsing_printing() {
    printf("Testing roundtrip label parsing and printing...\n");
    
    // Test a variety of valid labels
    test_parse_print_roundtrip("label_1:");
    test_parse_print_roundtrip("label_2 :");
    test_parse_print_roundtrip("  label_3:");
    test_parse_print_roundtrip("_label:");
    test_parse_print_roundtrip("$label:");
    test_parse_print_roundtrip("%label:");
    
    printf("Roundtrip label parsing and printing tests passed!\n");
}

void test_file_output() {
    printf("Testing label printing to file...\n");
    
    // Create a test label
    ptx_label_t test_label;
    test_label.name = strdup("file_test_label");
    
    // Create a temporary file
    FILE* temp_file = tmpfile();
    assert(temp_file != NULL);
    
    // Print the label to the file
    assert(print_label_to_file(temp_file, &test_label) == true);
    
    // Rewind the file to read it back
    rewind(temp_file);
    
    // Read back the file contents
    char buffer[BUFFER_SIZE] = {0};
    fgets(buffer, BUFFER_SIZE, temp_file);
    
    // Verify the contents
    assert(strcmp(buffer, "file_test_label:") == 0);
    
    // Clean up
    fclose(temp_file);
    free(test_label.name);
    
    printf("File output test passed!\n");
}

int main() {
    printf("Running label printer tests...\n");
    
    test_basic_label_printing();
    test_roundtrip_parsing_printing();
    test_file_output();
    
    printf("All label printer tests passed!\n");
    return 0;
} 