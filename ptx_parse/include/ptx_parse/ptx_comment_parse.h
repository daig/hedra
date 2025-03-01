#pragma once

#include <ptx_ast/comment.h>
#include <stdbool.h>

/**
 * Parse a comment from the input string.
 * 
 * @param input The input string to parse.
 * @param pos A pointer to the current position in the input string. Will be updated to point after the parsed comment.
 * @param comment A pointer to a comment_t pointer where the parsed comment will be stored.
 * @return true if a comment was successfully parsed, false otherwise.
 */
bool parse_comment(const char* input, int* pos, comment_t** comment);

/**
 * Free a comment AST node.
 */
void free_comment(comment_t* comment); 