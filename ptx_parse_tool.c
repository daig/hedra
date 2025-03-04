#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ptx_parse/ptx_code_block_parse.h"
#include "ptx_print/ptx_code_block_print.h"
#include "ptx_print/ptx_statement_print.h"
#include "ptx_ast/ptx_code_block.h"
#include "prelude/prelude.h"

/**
 * Read a file into a buffer
 * 
 * @param filename The name of the file to read
 * @return A buffer containing the file contents, or NULL on error
 */
char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return NULL;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Allocate buffer for file contents
    char* buffer = (char*)malloc(file_size + 1);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return NULL;
    }
    
    // Read file into buffer
    size_t read_size = fread(buffer, 1, file_size, file);
    if (read_size != (size_t)file_size) {
        fprintf(stderr, "Error reading file: %s\n", filename);
        free(buffer);
        fclose(file);
        return NULL;
    }
    
    // Null-terminate the buffer
    buffer[file_size] = '\0';
    
    fclose(file);
    return buffer;
}

/**
 * Parse a PTX file and print only the reconstructed code
 * 
 * @param filename The name of the file to parse
 * @return true on success, false on failure
 */
bool parse_and_print_ptx(const char* filename) {
    char* file_contents = read_file(filename);
    if (!file_contents) {
        return false;
    }
    
    // Extract the file header (everything before the first code block)
    char* code_block_start = strstr(file_contents, "{");
    if (!code_block_start) {
        fprintf(stderr, "Error: No code block found in file\n");
        free(file_contents);
        return false;
    }
    
    // Copy and print the header part
    size_t prefix_length = code_block_start - file_contents;
    char* prefix = (char*)malloc(prefix_length + 1);
    if (prefix) {
        strncpy(prefix, file_contents, prefix_length);
        prefix[prefix_length] = '\0';
        printf("%s", prefix);
        free(prefix);
    }
    
    // Parse the code block
    size_t consumed = 0;
    ptx_code_block_t* block = NULL;
    bool result = ptx_parse_code_block(code_block_start, &consumed, &block);
    
    if (!result || !block) {
        fprintf(stderr, "Failed to parse code block\n");
        free(file_contents);
        return false;
    }
    
    // Print just the reconstructed code block
    printf("{\n");
    for (size_t i = 0; i < block->num_statements; i++) {
        char statement_buffer[1024];
        print_statement_to_buffer(statement_buffer, sizeof(statement_buffer), block->statements[i]);
        printf("    %s\n", statement_buffer);
    }
    printf("}\n");
    
    // Free the block
    ptx_code_block_free(block);
    free(file_contents);
    
    return true;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <ptx_file>\n", argv[0]);
        return 1;
    }
    
    bool success = parse_and_print_ptx(argv[1]);
    return success ? 0 : 1;
} 