#include <ptx_parse/ptx_statement_parse.h>
#include <ptx_print/ptx_statement_print.h>
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
    if (statement->tag == INSTRUCTION && statement->instruction.predicate) {
        free(statement->instruction.predicate->name);
        free(statement->instruction.predicate);
    }
    free(statement->original_text);
    free(statement);
    
    printf("Roundtrip test passed for statement: '%s'\n", statement_str);
}

void test_predicated_statements() {
    printf("Testing predicated statements parsing and printing...\n");
    
    // Test basic predicated statements
    test_parse_print_roundtrip("@%p_cond bra $LABEL");
    test_parse_print_roundtrip("@!%p_out_of_bounds bra $RETURN");
    test_parse_print_roundtrip("@%p1 add.s32 %r0, %r1, %r2");
    test_parse_print_roundtrip("@!%p2 mov.u32 %r0, %r1");
    
    printf("Predicated statements tests passed!\n");
}

void test_labeled_predicated_statements() {
    printf("Testing labeled predicated statements parsing and printing...\n");
    
    // Test labeled predicated statements
    test_parse_print_roundtrip("loop_start: @%p_cond bra $LABEL");
    test_parse_print_roundtrip("exit_point: @!%p_out_of_bounds bra $RETURN");
    
    printf("Labeled predicated statements tests passed!\n");
}

void test_manual_predicated_statement_creation() {
    printf("Testing manual creation of predicated statements...\n");
    
    // Create a predicated instruction
    ptx_instruction_t instruction = {
        .tag = BRA,
        .predicate = (ptx_predicate_t*)malloc(sizeof(ptx_predicate_t))
    };
    instruction.predicate->name = strdup("%p_cond");
    instruction.predicate->negated = false;
    
    // Create a statement with the predicated instruction
    ptx_statement_t statement = {
        .tag = INSTRUCTION,
        .instruction = instruction,
        .label = {.name = NULL},
        .original_text = NULL
    };
    
    // Print the statement
    char buffer[BUFFER_SIZE] = {0};
    int printed = print_statement_to_buffer(buffer, BUFFER_SIZE, &statement);
    assert(printed > 0);
    
    // Verify the output
    printf("Printed statement: '%s'\n", buffer);
    assert(strstr(buffer, "@%p_cond") != NULL);
    assert(strstr(buffer, "bra") != NULL);
    
    // Clean up
    free(instruction.predicate->name);
    free(instruction.predicate);
    
    printf("Manual predicated statement creation test passed!\n");
}

int main() {
    printf("=== PTX Predicated Statement Tests ===\n");
    
    test_predicated_statements();
    test_labeled_predicated_statements();
    test_manual_predicated_statement_creation();
    
    printf("All predicated statement tests passed!\n");
    return 0;
} 