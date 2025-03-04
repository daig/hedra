/**
 * @file ptx_code_block.c
 * @brief Implementation of PTX code block AST components
 */

#include <stdlib.h>
#include <string.h>
#include "ptx_ast/ptx_code_block.h"

// Initial capacity for the statements array
#define INITIAL_CAPACITY 8

/**
 * Initialize a new code block
 * @return Newly allocated code block with an empty statement array
 */
ptx_code_block_t* ptx_code_block_init(void) {
    ptx_code_block_t* block = (ptx_code_block_t*)malloc(sizeof(ptx_code_block_t));
    if (block == NULL) {
        return NULL;
    }

    block->statements = (ptx_statement_t**)malloc(INITIAL_CAPACITY * sizeof(ptx_statement_t*));
    if (block->statements == NULL) {
        free(block);
        return NULL;
    }

    block->num_statements = 0;
    block->capacity = INITIAL_CAPACITY;
    return block;
}

/**
 * Add a statement to a code block
 * @param block The code block to add the statement to
 * @param statement The statement to add
 * @return true if successful, false otherwise
 */
bool ptx_code_block_add_statement(ptx_code_block_t* block, ptx_statement_t* statement) {
    if (block == NULL || statement == NULL) {
        return false;
    }

    // Resize the array if needed
    if (block->num_statements >= block->capacity) {
        size_t new_capacity = block->capacity * 2;
        ptx_statement_t** new_statements = (ptx_statement_t**)realloc(
            block->statements, new_capacity * sizeof(ptx_statement_t*));
        
        if (new_statements == NULL) {
            return false;
        }

        block->statements = new_statements;
        block->capacity = new_capacity;
    }

    // Add the statement
    block->statements[block->num_statements++] = statement;
    return true;
}

/**
 * Free all resources associated with a code block
 * @param block The code block to free
 */
void ptx_code_block_free(ptx_code_block_t* block) {
    if (block == NULL) {
        return;
    }

    // Free each statement
    for (size_t i = 0; i < block->num_statements; i++) {
        // Note: the statement freeing responsibility is left to the caller
        // as statements might be shared or owned by other parts of the AST
    }

    // Free the array and the block
    free(block->statements);
    free(block);
} 