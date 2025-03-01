#include <ptx_parse/ptx_statement_parse.h>
#include <ptx_ast/ptx_statement.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void test_directive_statements() {
    printf("Testing directive statements parsing...\n");
    
    struct ptx_statement_t* statement = NULL;
    
    // Test basic directive statement
    assert(parse_statement(".version 7.0", &statement) == true);
    assert(statement->tag == DIRECTIVE);
    assert(statement->directive == PTX_DIRECTIVE_VERSION);
    assert(statement->label.name == NULL);
    free(statement->original_text);
    free(statement);
    
    // Test directive statement with leading whitespace
    assert(parse_statement("   .target sm_70", &statement) == true);
    assert(statement->tag == DIRECTIVE);
    assert(statement->directive == PTX_DIRECTIVE_TARGET);
    assert(statement->label.name == NULL);
    free(statement->original_text);
    free(statement);
    
    printf("Directive statements parsing tests passed!\n");
}

void test_instruction_statements() {
    printf("Testing instruction statements parsing...\n");
    
    struct ptx_statement_t* statement = NULL;
    
    // Test basic instruction statement
    assert(parse_statement("add.s32 %r0, %r1, %r2", &statement) == true);
    assert(statement->tag == INSTRUCTION);
    assert(statement->instruction == ADD);
    assert(statement->label.name == NULL);
    free(statement->original_text);
    free(statement);
    
    // Test instruction statement with leading whitespace
    assert(parse_statement("   mov.u32 %r0, %r1", &statement) == true);
    assert(statement->tag == INSTRUCTION);
    assert(statement->instruction == MOV);
    assert(statement->label.name == NULL);
    free(statement->original_text);
    free(statement);
    
    printf("Instruction statements parsing tests passed!\n");
}

void test_labeled_statements() {
    printf("Testing labeled statements parsing...\n");
    
    struct ptx_statement_t* statement = NULL;
    
    // Test labeled directive statement
    assert(parse_statement("my_label: .entry kernel()", &statement) == true);
    assert(statement->tag == DIRECTIVE);
    assert(statement->directive == PTX_DIRECTIVE_ENTRY);
    assert(statement->label.name != NULL);
    assert(strcmp(statement->label.name, "my_label") == 0);
    free(statement->label.name);
    free(statement->original_text);
    free(statement);
    
    // Test labeled instruction statement
    assert(parse_statement("loop_start: add.s32 %r0, %r1, %r2", &statement) == true);
    assert(statement->tag == INSTRUCTION);
    assert(statement->instruction == ADD);
    assert(statement->label.name != NULL);
    assert(strcmp(statement->label.name, "loop_start") == 0);
    free(statement->label.name);
    free(statement->original_text);
    free(statement);
    
    // Test labeled statement with spacing
    assert(parse_statement("end:   ret;", &statement) == true);
    assert(statement->tag == INSTRUCTION);
    assert(statement->instruction == RET);
    assert(statement->label.name != NULL);
    assert(strcmp(statement->label.name, "end") == 0);
    free(statement->label.name);
    free(statement->original_text);
    free(statement);
    
    printf("Labeled statements parsing tests passed!\n");
}

void test_invalid_statements() {
    printf("Testing invalid statements parsing...\n");
    
    struct ptx_statement_t* statement = NULL;
    
    // Test empty string
    assert(parse_statement("", &statement) == false);
    
    // Test whitespace only
    assert(parse_statement("   ", &statement) == false);
    
    // Test null pointers
    assert(parse_statement(NULL, &statement) == false);
    assert(parse_statement(".version 7.0", NULL) == false);
    
    // Test invalid directive
    assert(parse_statement(".invalidirectivename", &statement) == false);
    
    // Test invalid instruction
    assert(parse_statement("notaninstruction %r0, %r1", &statement) == false);
    
    printf("Invalid statements parsing tests passed!\n");
}

int main() {
    printf("Running PTX statement parsing tests...\n");
    
    test_directive_statements();
    test_instruction_statements();
    test_labeled_statements();
    test_invalid_statements();
    
    printf("All statement parsing tests passed!\n");
    return 0;
} 