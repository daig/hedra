/**
 * @file ptx_code_block_print.c
 * @brief Implementation of PTX code block printer
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "ptx_print/ptx_code_block_print.h"
#include "ptx_print/ptx_statement_print.h"

/**
 * Print indentation spaces
 * 
 * @param out The output file
 * @param indent The number of spaces to print
 * @return true if successful, false otherwise
 */
static bool print_indent(FILE* out, int indent) {
    for (int i = 0; i < indent; i++) {
        if (fprintf(out, " ") < 0) {
            return false;
        }
    }
    return true;
}

/**
 * Print a PTX code block to a file
 * 
 * @param out The output file
 * @param block The code block to print
 * @param indent The indentation level (number of spaces)
 * @return true if successful, false otherwise
 */
bool ptx_print_code_block(FILE* out, const ptx_code_block_t* block, int indent) {
    if (!out || !block) {
        return false;
    }

    // Print opening brace
    if (fprintf(out, "{\n") < 0) {
        return false;
    }

    // Print each statement with indentation
    for (size_t i = 0; i < block->num_statements; i++) {
        // Print indentation
        if (!print_indent(out, indent + 4)) {
            return false;
        }

        // Print the statement
        if (!print_statement_to_file(out, block->statements[i])) {
            return false;
        }

        // Print newline after each statement
        if (fprintf(out, "\n") < 0) {
            return false;
        }
    }

    // Print closing brace with indentation
    if (!print_indent(out, indent) || fprintf(out, "}") < 0) {
        return false;
    }

    return true;
}

/**
 * Print a PTX code block to a buffer
 * 
 * @param buffer The buffer to print to
 * @param buffer_size The size of the buffer
 * @param block The code block to print
 * @param indent The indentation level (number of spaces)
 * @return The number of characters printed, or negative on error
 */
int ptx_print_code_block_to_buffer(char* buffer, size_t buffer_size, const ptx_code_block_t* block, int indent) {
    if (!buffer || buffer_size == 0 || !block) {
        return -1;
    }

    // Create a temporary file in memory
    FILE* memfile = fmemopen(buffer, buffer_size, "w");
    if (!memfile) {
        return -1;
    }

    // Print to the memory file
    bool success = ptx_print_code_block(memfile, block, indent);
    
    // Get the number of bytes written
    int bytes_written = ftell(memfile);
    
    // Close the memory file
    fclose(memfile);

    if (!success) {
        return -1;
    }

    return bytes_written;
} 