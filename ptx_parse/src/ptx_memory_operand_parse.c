#include <ptx_parse/ptx_memory_operand_parse.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Parse a variable or register name from a string
 * 
 * @param str The string to parse
 * @param name Pointer to store the parsed name
 * @param chars_read Pointer to store the number of characters read
 * @return true if parsing was successful, false otherwise
 */
static bool parse_name(const char* str, char** name, int* chars_read) {
    if (!str || !name || !chars_read) {
        return false;
    }
    
    // Skip leading whitespace
    const char* current = str;
    while (*current && isspace(*current)) {
        current++;
    }
    
    // Check if we have a name
    if (!*current || (*current != '%' && !isalpha(*current))) {
        return false;
    }
    
    // Find the end of the name
    const char* name_start = current;
    while (*current && (isalnum(*current) || *current == '_' || *current == '%' || *current == '.')) {
        current++;
    }
    
    // Create a copy of the name
    size_t name_length = current - name_start;
    *name = (char*)malloc(name_length + 1);
    if (!*name) {
        return false;
    }
    
    strncpy(*name, name_start, name_length);
    (*name)[name_length] = '\0';
    
    // Update the number of characters read
    *chars_read = current - str;
    
    return true;
}

/**
 * Parse an integer from a string
 * 
 * @param str The string to parse
 * @param value Pointer to store the parsed value
 * @param chars_read Pointer to store the number of characters read
 * @return true if parsing was successful, false otherwise
 */
static bool parse_integer(const char* str, int* value, int* chars_read) {
    if (!str || !value || !chars_read) {
        return false;
    }
    
    // Skip leading whitespace
    const char* current = str;
    while (*current && isspace(*current)) {
        current++;
    }
    
    // Check if we have a sign
    bool negative = false;
    if (*current == '-') {
        negative = true;
        current++;
    } else if (*current == '+') {
        current++;
    }
    
    // Check if we have a digit
    if (!*current || !isdigit(*current)) {
        return false;
    }
    
    // Parse the integer
    *value = 0;
    while (*current && isdigit(*current)) {
        *value = *value * 10 + (*current - '0');
        current++;
    }
    
    // Apply the sign
    if (negative) {
        *value = -*value;
    }
    
    // Update the number of characters read
    *chars_read = current - str;
    
    return true;
}

/**
 * Parse a memory operand from a string
 */
bool parse_memory_operand(const char* str, ptx_memory_operand_t** operand, int* chars_read) {
    if (!str || !operand || !chars_read) {
        return false;
    }
    
    // Initialize the number of characters read
    *chars_read = 0;
    
    // Skip leading whitespace
    const char* current = str;
    while (*current && isspace(*current)) {
        current++;
    }
    
    // Check if we have a memory operand
    if (!*current) {
        return false;
    }
    
    // Check if we have a bracketed memory operand
    if (*current == '[') {
        // Move past the opening bracket
        current++;
        
        // Skip whitespace
        while (*current && isspace(*current)) {
            current++;
        }
        
        // Check if we have an immediate address
        if (isdigit(*current)) {
            // Parse the immediate address
            unsigned int imm_addr = 0;
            while (*current && isdigit(*current)) {
                imm_addr = imm_addr * 10 + (*current - '0');
                current++;
            }
            
            // Skip whitespace
            while (*current && isspace(*current)) {
                current++;
            }
            
            // Check for closing bracket
            if (*current != ']') {
                return false;
            }
            
            // Move past the closing bracket
            current++;
            
            // Create the memory operand
            *operand = create_memory_operand(MEMORY_IMM_ADDR);
            if (!*operand) {
                return false;
            }
            
            // Set the immediate address
            (*operand)->imm_addr = imm_addr;
            
            // Update the number of characters read
            *chars_read = current - str;
            
            return true;
        }
        
        // Parse the base name
        char* base_name = NULL;
        int name_chars_read = 0;
        if (!parse_name(current, &base_name, &name_chars_read)) {
            return false;
        }
        
        // Update the current position
        current += name_chars_read;
        
        // Skip whitespace
        while (*current && isspace(*current)) {
            current++;
        }
        
        // Check if we have an offset
        if (*current == '+') {
            // Move past the plus sign
            current++;
            
            // Skip whitespace
            while (*current && isspace(*current)) {
                current++;
            }
            
            // Parse the offset
            int offset = 0;
            int offset_chars_read = 0;
            if (!parse_integer(current, &offset, &offset_chars_read)) {
                free(base_name);
                return false;
            }
            
            // Update the current position
            current += offset_chars_read;
            
            // Skip whitespace
            while (*current && isspace(*current)) {
                current++;
            }
            
            // Check for closing bracket
            if (*current != ']') {
                free(base_name);
                return false;
            }
            
            // Move past the closing bracket
            current++;
            
            // Determine if the base name is a register or variable
            bool is_register = (base_name[0] == '%');
            
            // Create the memory operand
            *operand = create_memory_operand(is_register ? MEMORY_REG_OFFSET : MEMORY_VAR_OFFSET);
            if (!*operand) {
                free(base_name);
                return false;
            }
            
            // Set the base name
            (*operand)->base_name = base_name;
            
            // Set the offset
            (*operand)->has_offset = true;
            (*operand)->offset = offset;
        } else if (*current == ']') {
            // Move past the closing bracket
            current++;
            
            // Determine if the base name is a register or variable
            bool is_register = (base_name[0] == '%');
            
            // Create the memory operand
            *operand = create_memory_operand(is_register ? MEMORY_REG : MEMORY_VAR);
            if (!*operand) {
                free(base_name);
                return false;
            }
            
            // Set the base name
            (*operand)->base_name = base_name;
        } else {
            // Invalid syntax
            free(base_name);
            return false;
        }
        
        // Update the number of characters read
        *chars_read = current - str;
        
        return true;
    } else {
        // Parse the base name
        char* base_name = NULL;
        int name_chars_read = 0;
        if (!parse_name(current, &base_name, &name_chars_read)) {
            return false;
        }
        
        // Update the current position
        current += name_chars_read;
        
        // Skip whitespace
        while (*current && isspace(*current)) {
            current++;
        }
        
        // Check if we have an array element
        if (*current == '[') {
            // Move past the opening bracket
            current++;
            
            // Skip whitespace
            while (*current && isspace(*current)) {
                current++;
            }
            
            // Parse the offset
            int offset = 0;
            int offset_chars_read = 0;
            if (!parse_integer(current, &offset, &offset_chars_read)) {
                free(base_name);
                return false;
            }
            
            // Update the current position
            current += offset_chars_read;
            
            // Skip whitespace
            while (*current && isspace(*current)) {
                current++;
            }
            
            // Check for closing bracket
            if (*current != ']') {
                free(base_name);
                return false;
            }
            
            // Move past the closing bracket
            current++;
            
            // Create the memory operand
            *operand = create_memory_operand(MEMORY_ARRAY_ELEMENT);
            if (!*operand) {
                free(base_name);
                return false;
            }
            
            // Set the base name
            (*operand)->base_name = base_name;
            
            // Set the offset
            (*operand)->has_offset = true;
            (*operand)->offset = offset;
            
            // Update the number of characters read
            *chars_read = current - str;
            
            return true;
        } else {
            // Not a memory operand
            free(base_name);
            return false;
        }
    }
} 