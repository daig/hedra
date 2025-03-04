#include <ptx_ast/ptx_memory_operand.h>
#include <stdlib.h>

/**
 * Create a memory operand of the specified type
 */
ptx_memory_operand_t* create_memory_operand(ptx_memory_operand_type type) {
    ptx_memory_operand_t* operand = (ptx_memory_operand_t*)malloc(sizeof(ptx_memory_operand_t));
    if (!operand) {
        return NULL;
    }
    
    // Initialize the operand
    operand->type = type;
    operand->base_name = NULL;
    operand->has_offset = false;
    operand->offset = 0;
    operand->imm_addr = 0;
    
    return operand;
}

/**
 * Free a memory operand
 */
void free_memory_operand(ptx_memory_operand_t* operand) {
    if (!operand) {
        return;
    }
    
    // Free the base name if it exists
    if (operand->base_name) {
        free(operand->base_name);
    }
    
    // Free the operand itself
    free(operand);
} 