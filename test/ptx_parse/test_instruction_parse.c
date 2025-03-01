#include <ptx_parse/ptx_instruction_parse.h>
#include <ptx_ast/ptx_instruction.h>
#include <stdio.h>
#include <assert.h>

void test_basic_instructions() {
    printf("Testing basic instruction parsing...\n");
    
    ptx_instruction_t instruction;
    
    // Test common arithmetic instructions
    assert(parse_instruction("add", &instruction) == true);
    assert(instruction == ADD);
    
    assert(parse_instruction("sub", &instruction) == true);
    assert(instruction == SUB);
    
    assert(parse_instruction("mul", &instruction) == true);
    assert(instruction == MUL);
    
    assert(parse_instruction("div", &instruction) == true);
    assert(instruction == DIV);
    
    printf("Basic instruction parsing tests passed!\n");
}

void test_case_insensitivity() {
    printf("Testing case-insensitive instruction parsing...\n");
    
    ptx_instruction_t instruction;
    
    // Test various case combinations
    assert(parse_instruction("ADD", &instruction) == true);
    assert(instruction == ADD);
    
    assert(parse_instruction("Mul", &instruction) == true);
    assert(instruction == MUL);
    
    assert(parse_instruction("lD", &instruction) == true);
    assert(instruction == LD);
    
    printf("Case-insensitive instruction parsing tests passed!\n");
}

void test_memory_instructions() {
    printf("Testing memory-related instruction parsing...\n");
    
    ptx_instruction_t instruction;
    
    // Test memory-related instructions
    assert(parse_instruction("ld", &instruction) == true);
    assert(instruction == LD);
    
    assert(parse_instruction("st", &instruction) == true);
    assert(instruction == ST);
    
    assert(parse_instruction("atom", &instruction) == true);
    assert(instruction == ATOM);
    
    assert(parse_instruction("membar", &instruction) == true);
    assert(instruction == MEMBAR);
    
    printf("Memory-related instruction parsing tests passed!\n");
}

void test_control_flow_instructions() {
    printf("Testing control flow instruction parsing...\n");
    
    ptx_instruction_t instruction;
    
    // Test control flow instructions
    assert(parse_instruction("bra", &instruction) == true);
    assert(instruction == BRA);
    
    assert(parse_instruction("call", &instruction) == true);
    assert(instruction == CALL);
    
    assert(parse_instruction("ret", &instruction) == true);
    assert(instruction == RET);
    
    assert(parse_instruction("exit", &instruction) == true);
    assert(instruction == EXIT);
    
    printf("Control flow instruction parsing tests passed!\n");
}

void test_advanced_instructions() {
    printf("Testing advanced instruction parsing...\n");
    
    ptx_instruction_t instruction;
    
    // Test more complex instructions
    assert(parse_instruction("mad", &instruction) == true);
    assert(instruction == MAD);
    
    assert(parse_instruction("fma", &instruction) == true);
    assert(instruction == FMA);
    
    assert(parse_instruction("cvt", &instruction) == true);
    assert(instruction == CVT);
    
    assert(parse_instruction("selp", &instruction) == true);
    assert(instruction == SELP);
    
    assert(parse_instruction("setp", &instruction) == true);
    assert(instruction == SETP);
    
    printf("Advanced instruction parsing tests passed!\n");
}

void test_invalid_instructions() {
    printf("Testing invalid instruction parsing...\n");
    
    ptx_instruction_t instruction;
    
    // Test invalid instructions
    assert(parse_instruction("nonexistent", &instruction) == false);
    assert(parse_instruction("", &instruction) == false);
    assert(parse_instruction(NULL, &instruction) == false);
    assert(parse_instruction("add", NULL) == false);
    
    // Test with leading/trailing spaces (should not be valid in our parser)
    assert(parse_instruction(" add", &instruction) == false);
    assert(parse_instruction("add ", &instruction) == false);
    
    printf("Invalid instruction parsing tests passed!\n");
}

int main() {
    printf("Running PTX instruction parsing tests...\n");
    
    test_basic_instructions();
    test_case_insensitivity();
    test_memory_instructions();
    test_control_flow_instructions();
    test_advanced_instructions();
    test_invalid_instructions();
    
    printf("All instruction parsing tests passed!\n");
    return 0;
} 