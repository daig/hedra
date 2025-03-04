/**
 * @file ptx_code_block.h
 * @brief Definitions for PTX code block AST components
 */

#ifndef PTX_CODE_BLOCK_H
#define PTX_CODE_BLOCK_H

#include <stdbool.h>
#include <stdint.h>
#include "ptx_ast/ptx_statement.h"

/**
 * Structure representing a PTX code block
 * A code block is a collection of statements enclosed in curly braces.
 */
typedef struct ptx_code_block {
    ptx_statement_t** statements;  // Array of statements in the block
    size_t num_statements;         // Number of statements
    size_t capacity;               // Capacity of the statements array
} ptx_code_block_t;

/**
 * Initialize a new code block
 * @return Newly allocated code block with an empty statement array
 */
ptx_code_block_t* ptx_code_block_init(void);

/**
 * Add a statement to a code block
 * @param block The code block to add the statement to
 * @param statement The statement to add
 * @return true if successful, false otherwise
 */
bool ptx_code_block_add_statement(ptx_code_block_t* block, ptx_statement_t* statement);

/**
 * Free all resources associated with a code block
 * @param block The code block to free
 */
void ptx_code_block_free(ptx_code_block_t* block);

#endif /* PTX_CODE_BLOCK_H */ 