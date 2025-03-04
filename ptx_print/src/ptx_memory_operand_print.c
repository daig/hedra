#include <ptx_print/ptx_memory_operand_print.h>
#include <string.h>

/**
 * Print a memory operand to a file stream
 */
bool print_memory_operand_to_file(FILE* file, const ptx_memory_operand_t* operand) {
    if (!file || !operand) {
        return false;
    }
    
    switch (operand->type) {
        case MEMORY_VAR:
            // [var]
            fprintf(file, "[%s]", operand->base_name);
            break;
            
        case MEMORY_REG:
            // [reg]
            fprintf(file, "[%s]", operand->base_name);
            break;
            
        case MEMORY_REG_OFFSET:
            // [reg+immOff]
            fprintf(file, "[%s+%d]", operand->base_name, operand->offset);
            break;
            
        case MEMORY_VAR_OFFSET:
            // [var+immOff]
            fprintf(file, "[%s+%d]", operand->base_name, operand->offset);
            break;
            
        case MEMORY_IMM_ADDR:
            // [immAddr]
            fprintf(file, "[%u]", operand->imm_addr);
            break;
            
        case MEMORY_ARRAY_ELEMENT:
            // var[immOff]
            fprintf(file, "%s[%d]", operand->base_name, operand->offset);
            break;
            
        default:
            return false;
    }
    
    return true;
}

/**
 * Print a memory operand to a string buffer
 */
int print_memory_operand_to_buffer(char* buffer, size_t buffer_size, const ptx_memory_operand_t* operand) {
    if (!buffer || buffer_size == 0 || !operand) {
        return -1;
    }
    
    int chars = 0;
    
    switch (operand->type) {
        case MEMORY_VAR:
            // [var]
            chars = snprintf(buffer, buffer_size, "[%s]", operand->base_name);
            break;
            
        case MEMORY_REG:
            // [reg]
            chars = snprintf(buffer, buffer_size, "[%s]", operand->base_name);
            break;
            
        case MEMORY_REG_OFFSET:
            // [reg+immOff]
            chars = snprintf(buffer, buffer_size, "[%s+%d]", operand->base_name, operand->offset);
            break;
            
        case MEMORY_VAR_OFFSET:
            // [var+immOff]
            chars = snprintf(buffer, buffer_size, "[%s+%d]", operand->base_name, operand->offset);
            break;
            
        case MEMORY_IMM_ADDR:
            // [immAddr]
            chars = snprintf(buffer, buffer_size, "[%u]", operand->imm_addr);
            break;
            
        case MEMORY_ARRAY_ELEMENT:
            // var[immOff]
            chars = snprintf(buffer, buffer_size, "%s[%d]", operand->base_name, operand->offset);
            break;
            
        default:
            return -1;
    }
    
    if (chars < 0 || chars >= buffer_size) {
        return -1;
    }
    
    return chars;
} 