#include <ptx_parse/c_directive_parse.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Determine the C directive type based on the directive name.
 * 
 * @param directive_name The name of the directive (without the # symbol).
 * @return The corresponding c_directive_t enum value, or -1 if unknown.
 */
static enum c_directive_t get_directive_type(const char* directive_name) {
    if (!directive_name) {
        return -1;
    }
    
    if (strcmp(directive_name, "include") == 0) {
        return C_DIRECTIVE_INCLUDE;
    } else if (strcmp(directive_name, "define") == 0) {
        return C_DIRECTIVE_DEFINE;
    } else if (strcmp(directive_name, "if") == 0) {
        return C_DIRECTIVE_IF;
    } else if (strcmp(directive_name, "ifdef") == 0) {
        return C_DIRECTIVE_IFDEF;
    } else if (strcmp(directive_name, "else") == 0) {
        return C_DIRECTIVE_ELSE;
    } else if (strcmp(directive_name, "endif") == 0) {
        return C_DIRECTIVE_ENDIF;
    } else if (strcmp(directive_name, "line") == 0) {
        return C_DIRECTIVE_LINE;
    } else if (strcmp(directive_name, "__FILE__") == 0) {
        return C_DIRECTIVE_FILE;
    }
    
    return -1;
}

/**
 * Parse a C-style directive from the input string.
 * 
 * @param input The input string to parse.
 * @param pos A pointer to the current position in the input string.
 * @param directive A pointer to a c_directive_t enum where the parsed directive type will be stored.
 * @param content A pointer to a char* where the directive content will be stored.
 * @return true if a C directive was successfully parsed, false otherwise.
 */
bool parse_c_directive(const char* input, int* pos, enum c_directive_t* directive, char** content) {
    if (!input || !pos || !directive || !content) {
        return false;
    }
    
    // Skip whitespace
    while (isspace(input[*pos])) {
        (*pos)++;
    }
    
    // Check if we have a '#' character
    if (input[*pos] != '#') {
        return false;
    }
    
    // Skip the '#' character
    (*pos)++;
    
    // Skip whitespace after '#'
    while (isspace(input[*pos])) {
        (*pos)++;
    }
    
    // Find the directive name
    int directive_start = *pos;
    while (isalnum(input[*pos]) || input[*pos] == '_') {
        (*pos)++;
    }
    
    if (*pos == directive_start) {
        // No directive name found
        return false;
    }
    
    // Extract the directive name
    int directive_length = *pos - directive_start;
    char* directive_name = (char*)malloc(directive_length + 1);
    if (!directive_name) {
        return false;
    }
    
    strncpy(directive_name, input + directive_start, directive_length);
    directive_name[directive_length] = '\0';
    
    // Determine the directive type
    enum c_directive_t dir_type = get_directive_type(directive_name);
    free(directive_name);  // We don't need the name anymore
    
    if (dir_type == -1) {
        // Unknown directive
        return false;
    }
    
    // Skip whitespace after directive name
    while (isspace(input[*pos])) {
        (*pos)++;
    }
    
    // Find the end of the directive (end of line)
    int content_start = *pos;
    while (input[*pos] && input[*pos] != '\n') {
        (*pos)++;
    }
    
    // Extract the directive content
    int content_length = *pos - content_start;
    *content = (char*)malloc(content_length + 1);
    if (!*content) {
        return false;
    }
    
    strncpy(*content, input + content_start, content_length);
    (*content)[content_length] = '\0';
    
    // Set the directive type
    *directive = dir_type;
    
    // Skip the newline if present
    if (input[*pos] == '\n') {
        (*pos)++;
    }
    
    return true;
}

/**
 * Free resources associated with a C directive AST node.
 * 
 * @param content The content string to free.
 */
void free_c_directive_content(char* content) {
    free(content);
} 