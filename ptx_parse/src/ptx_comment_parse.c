#include <ptx_parse/ptx_comment_parse.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Parse a C-style comment (/* ... *\/).
 */
static bool parse_c_style_comment(const char* input, int* pos, comment_t** comment) {
    // Check for the beginning of a C-style comment
    if (input[*pos] != '/' || input[*pos + 1] != '*') {
        return false;
    }

    // Skip the opening /*
    *pos += 2;
    
    // Find the end of the comment
    int start = *pos;
    while (input[*pos] && !(input[*pos] == '*' && input[*pos + 1] == '/')) {
        (*pos)++;
    }
    
    // Check if we reached the end without finding closing */
    if (!input[*pos]) {
        return false;
    }
    
    // Calculate the length of the comment text
    int length = *pos - start;
    
    // Allocate memory for the comment
    *comment = (comment_t*)malloc(sizeof(comment_t));
    if (!*comment) {
        return false;
    }
    
    // Allocate memory for the comment text
    (*comment)->text = (char*)malloc(length + 1);
    if (!(*comment)->text) {
        free(*comment);
        *comment = NULL;
        return false;
    }
    
    // Copy the comment text
    strncpy((*comment)->text, input + start, length);
    (*comment)->text[length] = '\0';
    
    // Skip the closing */
    *pos += 2;
    
    return true;
}

/**
 * Parse a C++-style comment (// ...).
 */
static bool parse_cpp_style_comment(const char* input, int* pos, comment_t** comment) {
    // Check for the beginning of a C++-style comment
    if (input[*pos] != '/' || input[*pos + 1] != '/') {
        return false;
    }

    // Skip the opening //
    *pos += 2;
    
    // Find the end of the comment (end of line or end of input)
    int start = *pos;
    while (input[*pos] && input[*pos] != '\n') {
        (*pos)++;
    }
    
    // Calculate the length of the comment text
    int length = *pos - start;
    
    // Allocate memory for the comment
    *comment = (comment_t*)malloc(sizeof(comment_t));
    if (!*comment) {
        return false;
    }
    
    // Allocate memory for the comment text
    (*comment)->text = (char*)malloc(length + 1);
    if (!(*comment)->text) {
        free(*comment);
        *comment = NULL;
        return false;
    }
    
    // Copy the comment text
    strncpy((*comment)->text, input + start, length);
    (*comment)->text[length] = '\0';
    
    // Skip the newline if we're at one
    if (input[*pos] == '\n') {
        (*pos)++;
    }
    
    return true;
}

/**
 * Parse a comment from the input string.
 */
bool parse_comment(const char* input, int* pos, comment_t** comment) {
    if (!input || !pos || !comment) {
        return false;
    }
    
    // Skip whitespace
    while (isspace(input[*pos])) {
        (*pos)++;
    }
    
    // Try to parse a C-style comment
    if (parse_c_style_comment(input, pos, comment)) {
        return true;
    }
    
    // Try to parse a C++-style comment
    if (parse_cpp_style_comment(input, pos, comment)) {
        return true;
    }
    
    return false;
}

/**
 * Free a comment AST node.
 */
void free_comment(comment_t* comment) {
    if (comment) {
        free(comment->text);
        free(comment);
    }
} 