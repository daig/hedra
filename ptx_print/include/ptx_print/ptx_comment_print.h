#pragma once

#include <ptx_ast/comment.h>
#include <stdio.h>

/**
 * Print a comment AST node to a file.
 * 
 * @param comment The comment AST node to print.
 * @param out The file to print to.
 * @return 0 if successful, non-zero otherwise.
 */
int print_comment(const comment_t* comment, FILE* out);

/**
 * Print a comment AST node to a string.
 * 
 * @param comment The comment AST node to print.
 * @param buffer The buffer to print to.
 * @param size The size of the buffer.
 * @return The number of bytes written, or negative on error.
 */
int sprint_comment(const comment_t* comment, char* buffer, size_t size); 