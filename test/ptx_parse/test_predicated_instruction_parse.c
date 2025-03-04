#include <ptx_parse/ptx_instruction_parse.h>
#include <ptx_print/ptx_instruction_print.h>
#include <ptx_ast/ptx_instruction.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define BUFFER_SIZE 256

// Function to test parsing a predicate
void test_predicate_parsing() {
    printf("Testing predicate parsing...\n");
    
    ptx_predicate_t* predicate = NULL;
    
    // Test basic predicate
    assert(parse_predicate("@%p_cond", &predicate) == true);
    assert(predicate != NULL);
    assert(predicate->name != NULL);
    assert(strcmp(predicate->name, "%p_cond") == 0);
    assert(predicate->negated == false);
    free(predicate->name);
    free(predicate);
    
    // Test negated predicate
    assert(parse_predicate("@!%p_out_of_bounds", &predicate) == true);
    assert(predicate != NULL);
    assert(predicate->name != NULL);
    assert(strcmp(predicate->name, "%p_out_of_bounds") == 0);
    assert(predicate->negated == true);
    free(predicate->name);
    free(predicate);
    
    // Test invalid predicates
    assert(parse_predicate("", &predicate) == false);
    assert(parse_predicate("@", &predicate) == false);
    assert(parse_predicate("@!", &predicate) == false);
    assert(parse_predicate("@p_cond", &predicate) == false); // Missing % symbol
    assert(parse_predicate("%p_cond", &predicate) == false); // Missing @ symbol
    
    printf("Predicate parsing tests passed!\n");
}

// Function to test printing a predicate
void test_predicate_printing() {
    printf("Testing predicate printing...\n");
    
    char buffer[BUFFER_SIZE];
    
    // Create and print a basic predicate
    ptx_predicate_t predicate1 = {
        .name = strdup("%p_cond"),
        .negated = false
    };
    
    int result = print_predicate_to_buffer(buffer, BUFFER_SIZE, &predicate1);
    assert(result > 0);
    assert(strncmp(buffer, "@%p_cond ", 9) == 0);
    
    // Create and print a negated predicate
    ptx_predicate_t predicate2 = {
        .name = strdup("%p_out_of_bounds"),
        .negated = true
    };
    
    result = print_predicate_to_buffer(buffer, BUFFER_SIZE, &predicate2);
    assert(result > 0);
    assert(strncmp(buffer, "@!%p_out_of_bounds ", 19) == 0);
    
    // Clean up
    free(predicate1.name);
    free(predicate2.name);
    
    printf("Predicate printing tests passed!\n");
}

// Function to test parsing a predicated instruction
void test_predicated_instruction_parsing() {
    printf("Testing predicated instruction parsing...\n");
    
    ptx_instruction_t* instruction = NULL;
    
    // Test basic predicated instruction
    assert(parse_full_instruction("@%p_cond bra $LABEL", &instruction) == true);
    assert(instruction != NULL);
    assert(instruction->tag == BRA);
    assert(instruction->predicate != NULL);
    assert(strcmp(instruction->predicate->name, "%p_cond") == 0);
    assert(instruction->predicate->negated == false);
    free(instruction->predicate->name);
    free(instruction->predicate);
    free(instruction);
    
    // Test negated predicated instruction
    assert(parse_full_instruction("@!%p_out_of_bounds bra $RETURN", &instruction) == true);
    assert(instruction != NULL);
    assert(instruction->tag == BRA);
    assert(instruction->predicate != NULL);
    assert(strcmp(instruction->predicate->name, "%p_out_of_bounds") == 0);
    assert(instruction->predicate->negated == true);
    free(instruction->predicate->name);
    free(instruction->predicate);
    free(instruction);
    
    // Test instruction without predicate
    assert(parse_full_instruction("add.s32 %r0, %r1, %r2", &instruction) == true);
    assert(instruction != NULL);
    assert(instruction->tag == ADD);
    assert(instruction->predicate == NULL);
    free(instruction);
    
    printf("Predicated instruction parsing tests passed!\n");
}

// Function to test printing a predicated instruction
void test_predicated_instruction_printing() {
    printf("Testing predicated instruction printing...\n");
    
    char buffer[BUFFER_SIZE];
    
    // Create and print a predicated instruction
    ptx_instruction_t instruction1 = {
        .tag = BRA,
        .predicate = (ptx_predicate_t*)malloc(sizeof(ptx_predicate_t))
    };
    instruction1.predicate->name = strdup("%p_cond");
    instruction1.predicate->negated = false;
    
    int result = print_instruction_to_buffer(buffer, BUFFER_SIZE, instruction1);
    assert(result > 0);
    assert(strncmp(buffer, "@%p_cond bra", 12) == 0);
    
    // Create and print a negated predicated instruction
    ptx_instruction_t instruction2 = {
        .tag = BRA,
        .predicate = (ptx_predicate_t*)malloc(sizeof(ptx_predicate_t))
    };
    instruction2.predicate->name = strdup("%p_out_of_bounds");
    instruction2.predicate->negated = true;
    
    result = print_instruction_to_buffer(buffer, BUFFER_SIZE, instruction2);
    assert(result > 0);
    assert(strncmp(buffer, "@!%p_out_of_bounds bra", 22) == 0);
    
    // Create and print an instruction without predicate
    ptx_instruction_t instruction3 = {
        .tag = ADD,
        .predicate = NULL
    };
    
    result = print_instruction_to_buffer(buffer, BUFFER_SIZE, instruction3);
    assert(result > 0);
    assert(strcmp(buffer, "add") == 0);
    
    // Clean up
    free(instruction1.predicate->name);
    free(instruction1.predicate);
    free(instruction2.predicate->name);
    free(instruction2.predicate);
    
    printf("Predicated instruction printing tests passed!\n");
}

// Function to test round-trip parsing and printing of predicated instructions
void test_predicated_instruction_roundtrip() {
    printf("Testing predicated instruction round-trip parsing and printing...\n");
    
    const char* test_cases[] = {
        "@%p_cond bra $LABEL",
        "@!%p_out_of_bounds bra $RETURN",
        "@%p1 add.s32 %r0, %r1, %r2",
        "@!%p2 mov.u32 %r0, %r1"
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        const char* original = test_cases[i];
        printf("Testing round-trip for: %s\n", original);
        
        // First parse
        ptx_instruction_t* parsed1 = NULL;
        assert(parse_full_instruction(original, &parsed1) == true);
        
        // Print to buffer
        char buffer[BUFFER_SIZE] = {0};
        int printed = print_instruction_to_buffer(buffer, BUFFER_SIZE, *parsed1);
        assert(printed > 0);
        
        // Parse again from the printed output
        ptx_instruction_t* parsed2 = NULL;
        assert(parse_full_instruction(buffer, &parsed2) == true);
        
        // Verify that both parsed instructions have the same tag
        assert(parsed1->tag == parsed2->tag);
        
        // Verify that both have predicates or both don't
        assert((parsed1->predicate != NULL) == (parsed2->predicate != NULL));
        
        // If they have predicates, verify they match
        if (parsed1->predicate != NULL && parsed2->predicate != NULL) {
            assert(strcmp(parsed1->predicate->name, parsed2->predicate->name) == 0);
            assert(parsed1->predicate->negated == parsed2->predicate->negated);
        }
        
        // Clean up
        if (parsed1->predicate) {
            free(parsed1->predicate->name);
            free(parsed1->predicate);
        }
        free(parsed1);
        
        if (parsed2->predicate) {
            free(parsed2->predicate->name);
            free(parsed2->predicate);
        }
        free(parsed2);
    }
    
    printf("Predicated instruction round-trip tests passed!\n");
}

int main() {
    printf("=== PTX Predicated Instruction Tests ===\n");
    
    test_predicate_parsing();
    test_predicate_printing();
    test_predicated_instruction_parsing();
    test_predicated_instruction_printing();
    test_predicated_instruction_roundtrip();
    
    printf("All predicated instruction tests passed!\n");
    return 0;
} 