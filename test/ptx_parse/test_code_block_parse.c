#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ptx_parse/ptx_code_block_parse.h"
#include "ptx_print/ptx_code_block_print.h"
#include "ptx_parse/ptx_statement_parse.h"
#include "prelude/prelude.h"

static int test_count = 0;
static int test_pass = 0;

#define TEST(name, code) \
    do { \
        printf("Test %d: %s - ", ++test_count, name); \
        if (code) { \
            printf("PASS\n"); \
            test_pass++; \
        } else { \
            printf("FAIL\n"); \
        } \
    } while (0)

/**
 * Test parsing a simple code block with a few statements
 */
bool test_simple_code_block() {
    const char* input = "{\n"
                        "    mov.u32 %r1, %tid.x;\n"
                        "    add.u32 %r2, %r1, 1;\n"
                        "    ret;\n"
                        "}";
    
    size_t consumed = 0;
    ptx_code_block_t* block = NULL;
    
    printf("Parsing: %s\n", input);
    bool result = ptx_parse_code_block(input, &consumed, &block);
    printf("Parse result: %s, consumed: %zu\n", result ? "true" : "false", consumed);
    
    if (!result || !block) {
        return false;
    }
    
    // Check the number of statements
    printf("Number of statements: %zu\n", block->num_statements);
    bool check_num_statements = block->num_statements == 3;
    
    // Check the statements
    bool check_statements = true;
    if (block->num_statements >= 1) {
        printf("Statement 1: %s\n", block->statements[0]->original_text);
        check_statements = check_statements && 
            strstr(block->statements[0]->original_text, "mov.u32") != NULL;
    }
    if (block->num_statements >= 2) {
        printf("Statement 2: %s\n", block->statements[1]->original_text);
        check_statements = check_statements && 
            strstr(block->statements[1]->original_text, "add.u32") != NULL;
    }
    if (block->num_statements >= 3) {
        printf("Statement 3: %s\n", block->statements[2]->original_text);
        check_statements = check_statements && 
            strstr(block->statements[2]->original_text, "ret") != NULL;
    }
    
    // Free the block
    ptx_code_block_free(block);
    
    return check_num_statements && check_statements;
}

/**
 * Test parsing a code block with comments and empty lines
 */
bool test_code_block_with_comments() {
    const char* input = "{\n"
                        "    // This is a comment\n"
                        "    mov.u32 %r1, %tid.x;\n"
                        "    \n"
                        "    /* This is a multi-line\n"
                        "       comment */\n"
                        "    add.u32 %r2, %r1, 1;\n"
                        "    ret;\n"
                        "}";
    
    size_t consumed = 0;
    ptx_code_block_t* block = NULL;
    
    printf("Parsing: %s\n", input);
    bool result = ptx_parse_code_block(input, &consumed, &block);
    printf("Parse result: %s, consumed: %zu\n", result ? "true" : "false", consumed);
    
    if (!result || !block) {
        return false;
    }
    
    // Check the number of statements
    printf("Number of statements: %zu\n", block->num_statements);
    bool check_num_statements = block->num_statements == 3;
    
    // Free the block
    ptx_code_block_free(block);
    
    return check_num_statements;
}

/**
 * Test round-trip parsing and printing of a code block
 */
bool test_code_block_round_trip() {
    const char* input = "{\n"
                        "    mov.u32 %r1, %tid.x;\n"
                        "    add.u32 %r2, %r1, 1;\n"
                        "    ret;\n"
                        "}";
    
    // First parse
    size_t consumed = 0;
    ptx_code_block_t* block = NULL;
    bool result = ptx_parse_code_block(input, &consumed, &block);
    
    if (!result || !block) {
        return false;
    }
    
    // Print to a buffer
    char buffer[1024];
    int printed = ptx_print_code_block_to_buffer(buffer, sizeof(buffer), block, 0);
    
    if (printed < 0) {
        ptx_code_block_free(block);
        return false;
    }
    
    printf("Printed code block:\n%s\n", buffer);
    
    // Parse the printed code again
    ptx_code_block_t* block2 = NULL;
    size_t consumed2 = 0;
    bool result2 = ptx_parse_code_block(buffer, &consumed2, &block2);
    
    if (!result2 || !block2) {
        ptx_code_block_free(block);
        return false;
    }
    
    // Check that the number of statements is the same
    bool check_num_statements = block->num_statements == block2->num_statements;
    printf("Number of statements: original=%zu, reparsed=%zu\n", 
           block->num_statements, block2->num_statements);
    
    // Free the blocks
    ptx_code_block_free(block);
    ptx_code_block_free(block2);
    
    return check_num_statements;
}

int main() {
    TEST("Simple code block", test_simple_code_block());
    TEST("Code block with comments", test_code_block_with_comments());
    TEST("Code block round-trip", test_code_block_round_trip());
    
    printf("\nTests: %d/%d passed\n", test_pass, test_count);
    return test_pass == test_count ? 0 : 1;
} 