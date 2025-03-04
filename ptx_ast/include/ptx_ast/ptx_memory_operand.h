#pragma once
#include <stdbool.h>

// Enum to represent the different types of memory operands
typedef enum ptx_memory_operand_type {
    MEMORY_VAR,           // [var]
    MEMORY_REG,           // [reg]
    MEMORY_REG_OFFSET,    // [reg+immOff]
    MEMORY_VAR_OFFSET,    // [var+immOff]
    MEMORY_IMM_ADDR,      // [immAddr]
    MEMORY_ARRAY_ELEMENT  // var[immOff]
} ptx_memory_operand_type;

// Structure to represent a memory operand
typedef struct ptx_memory_operand_t {
    ptx_memory_operand_type type;
    
    // Common fields
    char* base_name;      // Variable or register name
    
    // Fields for offset types
    bool has_offset;      // Whether there's an offset
    int offset;           // Immediate offset value
    
    // For immediate address
    unsigned int imm_addr; // Immediate address value (for MEMORY_IMM_ADDR)
} ptx_memory_operand_t;

// Function to create a memory operand
ptx_memory_operand_t* create_memory_operand(ptx_memory_operand_type type);

// Function to free a memory operand
void free_memory_operand(ptx_memory_operand_t* operand); 