#include <ptx_print/ptx_statement_print.h>
#include <ptx_parse/ptx_statement_parse.h>
#include <ptx_ast/ptx_statement.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 256

// Function to normalize whitespace for comparison
void normalize_whitespace(char* str) {
    char* src = str;
    char* dst = str;
    bool prev_was_space = true; // Start with true to handle leading whitespace
    
    // Skip leading whitespace
    while (*src && isspace(*src)) {
        src++;
    }
    
    // Compress multiple whitespace into a single space
    while (*src) {
        if (isspace(*src)) {
            if (!prev_was_space) {
                *dst++ = ' ';
                prev_was_space = true;
            }
        } else {
            *dst++ = *src;
            prev_was_space = false;
        }
        src++;
    }
    
    // Remove trailing whitespace
    if (dst > str && prev_was_space) {
        dst--;
    }
    
    *dst = '\0';
}

// Function to test round-trip parsing and printing
void test_parse_print_roundtrip(const char* statement_str) {
    ptx_statement_t* statement = NULL;
    char buffer[BUFFER_SIZE] = {0};
    char normalized_orig[BUFFER_SIZE] = {0};
    char normalized_printed[BUFFER_SIZE] = {0};
    
    // Make a copy of the original string for normalization
    strncpy(normalized_orig, statement_str, BUFFER_SIZE);
    normalized_orig[BUFFER_SIZE - 1] = '\0';
    normalize_whitespace(normalized_orig);
    
    // Parse the statement string
    assert(parse_statement(statement_str, &statement) == true);
    assert(statement != NULL);
    
    // Print the statement to the buffer
    int printed = print_statement_to_buffer(buffer, BUFFER_SIZE, statement);
    assert(printed > 0);
    
    // Normalize the printed output for comparison
    strncpy(normalized_printed, buffer, BUFFER_SIZE);
    normalized_printed[BUFFER_SIZE - 1] = '\0';
    normalize_whitespace(normalized_printed);
    
    // Check that the normalized strings match
    printf("Original (normalized): '%s'\n", normalized_orig);
    printf("Printed (normalized): '%s'\n", normalized_printed);
    assert(strcmp(normalized_printed, normalized_orig) == 0);
    
    // Clean up
    free(statement->original_text);
    free(statement);
    
    printf("Roundtrip test passed for statement: '%s'\n", statement_str);
}

void test_directive_statements() {
    printf("Testing directive statements printing...\n");
    
    // Test basic directive statements
    test_parse_print_roundtrip(".version 7.0");
    test_parse_print_roundtrip(".target sm_70");
    test_parse_print_roundtrip(".entry kernel()");
    
    printf("Directive statements printing tests passed!\n");
}

void test_instruction_statements() {
    printf("Testing instruction statements printing...\n");
    
    // Test basic instruction statements
    test_parse_print_roundtrip("add.s32 %r0, %r1, %r2");
    test_parse_print_roundtrip("mov.u32 %r0, %r1");
    test_parse_print_roundtrip("ret;");
    
    printf("Instruction statements printing tests passed!\n");
}

void test_labeled_statements() {
    printf("Testing labeled statements printing...\n");
    
    // Test labeled statements
    test_parse_print_roundtrip("my_label: .entry kernel()");
    test_parse_print_roundtrip("loop_start: add.s32 %r0, %r1, %r2");
    test_parse_print_roundtrip("end:   ret;");
    
    // Test with various spacing
    test_parse_print_roundtrip("label1:.entry kernel()");
    test_parse_print_roundtrip("label2 : add.s32 %r0, %r1, %r2");
    
    printf("Labeled statements printing tests passed!\n");
}

void test_file_output() {
    printf("Testing statement printing to file...\n");
    
    // Create statements for testing
    ptx_statement_t* directive_statement = NULL;
    ptx_statement_t* instruction_statement = NULL;
    ptx_statement_t* labeled_statement = NULL;
    
    assert(parse_statement(".version 7.0", &directive_statement) == true);
    assert(parse_statement("add.s32 %r0, %r1, %r2", &instruction_statement) == true);
    assert(parse_statement("label: ret;", &labeled_statement) == true);
    
    // Create a temporary file
    FILE* temp_file = tmpfile();
    assert(temp_file != NULL);
    
    // Print statements to the file
    assert(print_statement_to_file(temp_file, directive_statement) == true);
    fprintf(temp_file, "\n"); // Add separator
    assert(print_statement_to_file(temp_file, instruction_statement) == true);
    fprintf(temp_file, "\n"); // Add separator
    assert(print_statement_to_file(temp_file, labeled_statement) == true);
    
    // Rewind the file
    rewind(temp_file);
    
    // Read and check each line
    char buffer[BUFFER_SIZE];
    
    // First line should be directive
    assert(fgets(buffer, BUFFER_SIZE, temp_file) != NULL);
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline
    assert(strcmp(buffer, ".version 7.0") == 0);
    
    // Second line should be instruction
    assert(fgets(buffer, BUFFER_SIZE, temp_file) != NULL);
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline
    assert(strcmp(buffer, "add.s32 %r0, %r1, %r2") == 0);
    
    // Third line should be labeled instruction
    assert(fgets(buffer, BUFFER_SIZE, temp_file) != NULL);
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline
    assert(strcmp(buffer, "label: ret;") == 0);
    
    // Clean up
    fclose(temp_file);
    free(directive_statement->original_text);
    free(directive_statement);
    free(instruction_statement->original_text);
    free(instruction_statement);
    free(labeled_statement->label.name); // Free the label name
    free(labeled_statement->original_text);
    free(labeled_statement);
    
    printf("File output tests passed!\n");
}

int main() {
    printf("=== PTX Statement Printer Tests ===\n");
    
    test_directive_statements();
    test_instruction_statements();
    test_labeled_statements();
    test_file_output();
    
    printf("All statement printer tests passed!\n");
    return 0;
} 