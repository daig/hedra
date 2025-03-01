#include <ptx_parse/ptx_statespace_parse.h>
#include <ptx_print/ptx_statespace_print.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

// Test constants
#define BUFFER_SIZE 64

// Test parse function
void test_parse_state_space(const char* str, ptx_state_space_t expected_space) {
    ptx_state_space_t space;
    
    printf("Testing parse_state_space(\"%s\"): ", str);
    
    bool result = parse_state_space(str, &space);
    
    if (result && space == expected_space) {
        printf("PASS\n");
    } else {
        printf("FAIL (expected enum value %d, got %d)\n", expected_space, space);
        assert(result && "Parse should succeed");
        assert(space == expected_space && "Enum value should match expected");
    }
}

// Test print function (to buffer)
void test_print_state_space(ptx_state_space_t space, const char* expected_str) {
    char buffer[BUFFER_SIZE] = {0};
    
    printf("Testing print_state_space_to_buffer(%d): ", space);
    
    int result = print_state_space_to_buffer(buffer, BUFFER_SIZE, space);
    
    if (result > 0 && strcmp(buffer, expected_str) == 0) {
        printf("PASS\n");
    } else {
        printf("FAIL (expected string \"%s\", got \"%s\")\n", expected_str, buffer);
        assert(result > 0 && "Print should succeed");
        assert(strcmp(buffer, expected_str) == 0 && "Output string should match expected");
    }
}

// Test round-trip (parse -> print -> parse)
void test_round_trip(const char* original_str) {
    ptx_state_space_t first_space;
    char buffer[BUFFER_SIZE] = {0};
    ptx_state_space_t second_space;
    
    printf("Testing round-trip for \"%s\": ", original_str);
    
    // Parse original string to enum
    bool parse_result = parse_state_space(original_str, &first_space);
    assert(parse_result && "First parse should succeed");
    
    // Print enum to buffer
    int print_result = print_state_space_to_buffer(buffer, BUFFER_SIZE, first_space);
    assert(print_result > 0 && "Print should succeed");
    
    // Parse printed string back to enum
    bool parse_again_result = parse_state_space(buffer, &second_space);
    assert(parse_again_result && "Second parse should succeed");
    
    // Compare original and final enums
    if (first_space == second_space) {
        printf("PASS\n");
    } else {
        printf("FAIL (enums don't match: %d vs %d)\n", first_space, second_space);
        assert(first_space == second_space && "Round-trip should preserve enum value");
    }
}

int main() {
    printf("=== PTX State Space Parser/Printer Tests ===\n\n");
    
    // Test parsing
    printf("--- Parse Tests ---\n");
    test_parse_state_space(".reg", PTX_STATE_REG);
    test_parse_state_space(".sreg", PTX_STATE_SREG);
    test_parse_state_space(".const", PTX_STATE_CONST);
    test_parse_state_space(".global", PTX_STATE_GLOBAL);
    test_parse_state_space(".local", PTX_STATE_LOCAL);
    test_parse_state_space(".param", PTX_STATE_PARAM);
    test_parse_state_space(".shared", PTX_STATE_SHARED);
    test_parse_state_space(".tex", PTX_STATE_TEX);
    
    // Test with mixed case (should be case-insensitive)
    test_parse_state_space(".ReG", PTX_STATE_REG);
    test_parse_state_space(".GLOBal", PTX_STATE_GLOBAL);
    
    printf("\n--- Print Tests ---\n");
    test_print_state_space(PTX_STATE_REG, ".reg");
    test_print_state_space(PTX_STATE_SREG, ".sreg");
    test_print_state_space(PTX_STATE_CONST, ".const");
    test_print_state_space(PTX_STATE_GLOBAL, ".global");
    test_print_state_space(PTX_STATE_LOCAL, ".local");
    test_print_state_space(PTX_STATE_PARAM, ".param");
    test_print_state_space(PTX_STATE_SHARED, ".shared");
    test_print_state_space(PTX_STATE_TEX, ".tex");
    
    printf("\n--- Round-Trip Tests ---\n");
    test_round_trip(".reg");
    test_round_trip(".sreg");
    test_round_trip(".const");
    test_round_trip(".global");
    test_round_trip(".local");
    test_round_trip(".param");
    test_round_trip(".shared");
    test_round_trip(".tex");
    
    printf("\nAll tests passed!\n");
    return 0;
} 