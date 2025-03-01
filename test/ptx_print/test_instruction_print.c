#include <ptx_print/ptx_instruction_print.h>
#include <ptx_parse/ptx_instruction_parse.h>
#include <ptx_ast/ptx_instruction.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define BUFFER_SIZE 128

// Function to test round-trip parsing and printing
void test_parse_print_roundtrip(const char* instruction_str) {
    ptx_instruction_t instruction;
    char buffer[BUFFER_SIZE] = {0};
    
    // Parse the instruction string
    assert(parse_instruction(instruction_str, &instruction) == true);
    
    // Print the instruction to the buffer
    int printed = print_instruction_to_buffer(buffer, BUFFER_SIZE, instruction);
    assert(printed > 0);
    
    // Check that the printed string matches the original (modulo case)
    assert(strcasecmp(buffer, instruction_str) == 0);
    
    printf("Roundtrip test passed for instruction: '%s' -> '%s'\n", instruction_str, buffer);
}

void test_basic_instructions() {
    printf("Testing basic instruction printing...\n");
    
    // Test printing to buffer
    char buffer[BUFFER_SIZE];
    
    // Test ADD instruction
    ptx_instruction_t add_instruction = ADD;
    int result = print_instruction_to_buffer(buffer, BUFFER_SIZE, add_instruction);
    assert(result > 0);
    assert(strcmp(buffer, "add") == 0);
    
    // Test MUL instruction
    ptx_instruction_t mul_instruction = MUL;
    result = print_instruction_to_buffer(buffer, BUFFER_SIZE, mul_instruction);
    assert(result > 0);
    assert(strcmp(buffer, "mul") == 0);
    
    // Test LD instruction
    ptx_instruction_t ld_instruction = LD;
    result = print_instruction_to_buffer(buffer, BUFFER_SIZE, ld_instruction);
    assert(result > 0);
    assert(strcmp(buffer, "ld") == 0);
    
    printf("Basic instruction printing tests passed!\n");
}

void test_roundtrip_parsing_printing() {
    printf("Testing roundtrip parsing and printing...\n");
    
    // Test common instructions
    test_parse_print_roundtrip("add");
    test_parse_print_roundtrip("sub");
    test_parse_print_roundtrip("mul");
    test_parse_print_roundtrip("div");
    test_parse_print_roundtrip("ld");
    test_parse_print_roundtrip("st");
    test_parse_print_roundtrip("mov");
    test_parse_print_roundtrip("mad");
    test_parse_print_roundtrip("fma");
    test_parse_print_roundtrip("setp");
    test_parse_print_roundtrip("bra");
    test_parse_print_roundtrip("cvt");
    
    printf("Roundtrip parsing and printing tests passed!\n");
}

void test_file_output() {
    printf("Testing instruction printing to file...\n");
    
    // Open a temporary file for testing
    FILE* temp_file = tmpfile();
    assert(temp_file != NULL);
    
    // Print instructions to the file
    assert(print_instruction_to_file(temp_file, ADD) == true);
    assert(print_instruction_to_file(temp_file, MUL) == true);
    assert(print_instruction_to_file(temp_file, LD) == true);
    
    // Rewind the file to read from the beginning
    rewind(temp_file);
    
    // Read and check the contents
    char buffer[BUFFER_SIZE];
    assert(fgets(buffer, BUFFER_SIZE, temp_file) != NULL);
    assert(strcmp(buffer, "addmulld") == 0);
    
    // Close the file
    fclose(temp_file);
    
    printf("File output tests passed!\n");
}

// Test with case differences to ensure they match (modulo case)
void test_case_insensitivity() {
    printf("Testing case insensitivity in roundtrip parsing and printing...\n");
    
    ptx_instruction_t instruction;
    char buffer[BUFFER_SIZE] = {0};
    
    // Test with uppercase in original
    assert(parse_instruction("ADD", &instruction) == true);
    assert(print_instruction_to_buffer(buffer, BUFFER_SIZE, instruction) > 0);
    // Note: Our printer always outputs lowercase
    assert(strcmp(buffer, "add") == 0);
    
    // Test with mixed case
    assert(parse_instruction("MuL", &instruction) == true);
    assert(print_instruction_to_buffer(buffer, BUFFER_SIZE, instruction) > 0);
    assert(strcmp(buffer, "mul") == 0);
    
    printf("Case insensitivity tests passed!\n");
}

int main() {
    printf("=== PTX Instruction Printer Tests ===\n");
    
    test_basic_instructions();
    test_roundtrip_parsing_printing();
    test_file_output();
    test_case_insensitivity();
    
    printf("All instruction printer tests passed!\n");
    return 0;
} 