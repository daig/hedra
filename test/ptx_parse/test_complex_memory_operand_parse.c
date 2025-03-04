#include <ptx_parse/ptx_instruction_parse.h>
#include <ptx_print/ptx_instruction_print.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/**
 * Test parsing and printing of complex memory operands
 */
int main() {
    // Test case 1: ld.const.s32 q, [tbl+12];
    const char* test_str = "ld.const.s32 q, [tbl+12];";
    ptx_instruction_t* instruction = NULL;
    
    // Parse the instruction
    bool parse_result = parse_full_instruction(test_str, &instruction);
    assert(parse_result && "Failed to parse instruction");
    
    // Verify the instruction tag
    assert(instruction->tag == LD && "Incorrect instruction tag");
    
    // Verify the modifiers
    assert(instruction->modifiers != NULL && "Missing modifiers");
    assert(strcmp(instruction->modifiers->modifier, "s32") == 0 && "Incorrect modifier");
    assert(instruction->modifiers->next != NULL && "Missing second modifier");
    assert(strcmp(instruction->modifiers->next->modifier, "const") == 0 && "Incorrect second modifier");
    
    // Verify the destination operand
    assert(instruction->dest_operand != NULL && "Missing destination operand");
    assert(strcmp(instruction->dest_operand, "q") == 0 && "Incorrect destination operand");
    
    // Verify the source operand
    assert(instruction->num_operands == 1 && "Incorrect number of operands");
    assert(instruction->is_mem_operand[0] && "Operand should be a memory operand");
    
    // Verify the memory operand
    ptx_memory_operand_t* mem_operand = instruction->mem_operands[0];
    assert(mem_operand != NULL && "Missing memory operand");
    assert(mem_operand->type == MEMORY_VAR_OFFSET && "Incorrect memory operand type");
    assert(mem_operand->base_name != NULL && "Missing base name");
    assert(strcmp(mem_operand->base_name, "tbl") == 0 && "Incorrect base name");
    assert(mem_operand->has_offset && "Missing offset");
    assert(mem_operand->offset == 12 && "Incorrect offset");
    
    // Print the instruction to a buffer
    char buffer[256];
    int print_result = print_instruction_to_buffer(buffer, sizeof(buffer), *instruction);
    assert(print_result > 0 && "Failed to print instruction");
    
    // Verify the printed instruction
    printf("Original: %s\n", test_str);
    printf("Printed:  %s\n", buffer);
    
    // Parse the printed instruction
    ptx_instruction_t* instruction2 = NULL;
    bool parse_result2 = parse_full_instruction(buffer, &instruction2);
    assert(parse_result2 && "Failed to parse printed instruction");
    
    // Print the second instruction to a buffer
    char buffer2[256];
    int print_result2 = print_instruction_to_buffer(buffer2, sizeof(buffer2), *instruction2);
    assert(print_result2 > 0 && "Failed to print second instruction");
    
    // Verify the second printed instruction
    printf("Reprinted: %s\n", buffer2);
    
    // Verify that the two printed instructions are the same
    assert(strcmp(buffer, buffer2) == 0 && "Printed instructions do not match");
    
    // Free the instructions
    free_instruction(instruction);
    free_instruction(instruction2);
    
    printf("All tests passed!\n");
    return 0;
} 