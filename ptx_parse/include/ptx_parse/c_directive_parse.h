#pragma once

#include <ptx_ast/c_directive.h>
#include <stdbool.h>

/**
 * Parse a C-style directive (e.g., #include, #define) from the input string.
 * 
 * @param input The input string to parse.
 * @param pos A pointer to the current position in the input string. Will be updated to point after the parsed directive.
 * @param directive A pointer to an enum c_directive_t where the parsed directive type will be stored.
 * @param content A pointer to a char* where the directive content will be stored. The caller is responsible for freeing this memory.
 * @return true if a C directive was successfully parsed, false otherwise.
 */
bool parse_c_directive(const char* input, int* pos, enum c_directive_t* directive, char** content);

/**
 * Free resources associated with a C directive AST node.
 * 
 * @param content The content string to free.
 */
void free_c_directive_content(char* content); 