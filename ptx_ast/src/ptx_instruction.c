#include <ptx_ast/ptx_instruction.h>
#include <stdlib.h>
#include <string.h>

/**
 * Create a new instruction modifier
 */
ptx_instruction_modifier_t* create_instruction_modifier(const char* modifier_text) {
    if (!modifier_text) {
        return NULL;
    }
    
    ptx_instruction_modifier_t* modifier = (ptx_instruction_modifier_t*)malloc(sizeof(ptx_instruction_modifier_t));
    if (!modifier) {
        return NULL;
    }
    
    // Copy the modifier text
    modifier->modifier = strdup(modifier_text);
    if (!modifier->modifier) {
        free(modifier);
        return NULL;
    }
    
    modifier->next = NULL;
    return modifier;
}

/**
 * Free an instruction modifier list
 */
void free_instruction_modifiers(ptx_instruction_modifier_t* modifiers) {
    ptx_instruction_modifier_t* current = modifiers;
    while (current) {
        ptx_instruction_modifier_t* next = current->next;
        if (current->modifier) {
            free(current->modifier);
        }
        free(current);
        current = next;
    }
}

/**
 * Add a modifier to an instruction
 */
bool add_instruction_modifier(ptx_instruction_t* instruction, const char* modifier_text) {
    if (!instruction || !modifier_text) {
        return false;
    }
    
    ptx_instruction_modifier_t* new_modifier = create_instruction_modifier(modifier_text);
    if (!new_modifier) {
        return false;
    }
    
    // Add to the beginning of the list for simplicity
    new_modifier->next = instruction->modifiers;
    instruction->modifiers = new_modifier;
    
    return true;
}

/**
 * Add a register operand to an instruction
 */
bool add_register_operand(ptx_instruction_t* instruction, const char* operand_text) {
    if (!instruction || !operand_text || instruction->num_operands >= MAX_INSTRUCTION_OPERANDS) {
        return false;
    }
    
    // Copy the operand text
    instruction->reg_operands[instruction->num_operands] = strdup(operand_text);
    if (!instruction->reg_operands[instruction->num_operands]) {
        return false;
    }
    
    // Mark as not a memory operand
    instruction->is_mem_operand[instruction->num_operands] = false;
    
    // Increment the operand count
    instruction->num_operands++;
    
    return true;
}

/**
 * Add a memory operand to an instruction
 */
bool add_memory_operand(ptx_instruction_t* instruction, ptx_memory_operand_t* memory_operand) {
    if (!instruction || !memory_operand || instruction->num_operands >= MAX_INSTRUCTION_OPERANDS) {
        return false;
    }
    
    // Store the memory operand
    instruction->mem_operands[instruction->num_operands] = memory_operand;
    
    // Mark as a memory operand
    instruction->is_mem_operand[instruction->num_operands] = true;
    
    // Increment the operand count
    instruction->num_operands++;
    
    return true;
}

/**
 * Set the destination operand of an instruction
 */
bool set_instruction_dest(ptx_instruction_t* instruction, const char* dest_text) {
    if (!instruction || !dest_text) {
        return false;
    }
    
    // Free any existing destination
    if (instruction->dest_operand) {
        free(instruction->dest_operand);
    }
    
    // Copy the destination text
    instruction->dest_operand = strdup(dest_text);
    if (!instruction->dest_operand) {
        return false;
    }
    
    return true;
}

/**
 * Free an instruction
 */
void free_instruction(ptx_instruction_t* instruction) {
    if (!instruction) {
        return;
    }
    
    // Free the predicate if it exists
    if (instruction->predicate) {
        if (instruction->predicate->name) {
            free(instruction->predicate->name);
        }
        free(instruction->predicate);
    }
    
    // Free the modifiers
    free_instruction_modifiers(instruction->modifiers);
    
    // Free the destination operand
    if (instruction->dest_operand) {
        free(instruction->dest_operand);
    }
    
    // Free the operands
    for (int i = 0; i < instruction->num_operands; i++) {
        if (instruction->is_mem_operand[i]) {
            free_memory_operand(instruction->mem_operands[i]);
        } else if (instruction->reg_operands[i]) {
            free(instruction->reg_operands[i]);
        }
    }
    
    // Free the instruction itself
    free(instruction);
} 