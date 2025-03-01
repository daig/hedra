#include <ptx_print/ptx_statement_print.h>
#include <ptx_print/ptx_label_print.h>
#include <ptx_print/ptx_directive_print.h>
#include <ptx_print/ptx_instruction_print.h>
#include <string.h>

// Prints a PTX statement to a file stream
bool print_statement_to_file(FILE* file, const ptx_statement_t* statement) {
    if (!file || !statement) {
        return false;
    }
    
    // If we have the original text, use it
    if (statement->original_text) {
        fprintf(file, "%s", statement->original_text);
        return true;
    }
    
    // Fallback: Print label if it exists
    if (statement->label.name != NULL) {
        if (!print_label_to_file(file, &statement->label)) {
            return false;
        }
        // Add a space after the label
        fprintf(file, " ");
    }
    
    // Print directive or instruction based on statement tag
    if (statement->tag == DIRECTIVE) {
        return print_directive_to_file(file, statement->directive);
    } else if (statement->tag == INSTRUCTION) {
        return print_instruction_to_file(file, statement->instruction);
    }
    
    return false; // Unknown statement tag
}

// Prints a PTX statement to a string buffer
int print_statement_to_buffer(char* buffer, size_t buffer_size, const ptx_statement_t* statement) {
    if (!buffer || buffer_size == 0 || !statement) {
        return -1;
    }
    
    // If we have the original text, use it
    if (statement->original_text) {
        return snprintf(buffer, buffer_size, "%s", statement->original_text);
    }
    
    // Fallback implementation if original_text is not available
    // Create a temporary buffer for each part of the statement
    char temp_buffer[buffer_size];
    int total_chars = 0;
    
    // Initialize buffer to empty string
    buffer[0] = '\0';
    
    // Print label if it exists
    if (statement->label.name != NULL) {
        int label_chars = print_label_to_buffer(temp_buffer, buffer_size, &statement->label);
        if (label_chars < 0) {
            return -1;
        }
        
        // Copy label to the output buffer
        strncat(buffer, temp_buffer, buffer_size - total_chars - 1);
        total_chars += label_chars;
        
        // Add a space after the label if there's room
        if (total_chars < buffer_size - 1) {
            strncat(buffer, " ", buffer_size - total_chars - 1);
            total_chars += 1;
        }
    }
    
    // Print directive or instruction based on statement tag
    if (statement->tag == DIRECTIVE) {
        int directive_chars = print_directive_to_buffer(temp_buffer, buffer_size - total_chars, statement->directive);
        if (directive_chars < 0) {
            return -1;
        }
        strncat(buffer, temp_buffer, buffer_size - total_chars - 1);
        total_chars += directive_chars;
    } else if (statement->tag == INSTRUCTION) {
        int instruction_chars = print_instruction_to_buffer(temp_buffer, buffer_size - total_chars, statement->instruction);
        if (instruction_chars < 0) {
            return -1;
        }
        strncat(buffer, temp_buffer, buffer_size - total_chars - 1);
        total_chars += instruction_chars;
    } else {
        return -1; // Unknown statement tag
    }
    
    return total_chars;
} 