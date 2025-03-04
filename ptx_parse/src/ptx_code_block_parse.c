/**
 * @file ptx_code_block_parse.c
 * @brief Implementation of PTX code block parser
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "ptx_parse/ptx_code_block_parse.h"
#include "ptx_parse/ptx_statement_parse.h"
#include "ptx_ast/ptx_code_block.h"

/**
 * Helper function to skip whitespace and comments
 */
static size_t skip_whitespace_and_comments(const char* input) {
    size_t pos = 0;
    while (input[pos]) {
        // Skip whitespace (including newlines)
        if (isspace(input[pos])) {
            pos++;
            continue;
        }
        
        // Skip single-line comments
        if (input[pos] == '/' && input[pos+1] == '/') {
            pos += 2;
            while (input[pos] && input[pos] != '\n') {
                pos++;
            }
            if (input[pos] == '\n') {
                pos++;
            }
            continue;
        }
        
        // Skip multi-line comments
        if (input[pos] == '/' && input[pos+1] == '*') {
            pos += 2;
            while (input[pos] && !(input[pos] == '*' && input[pos+1] == '/')) {
                pos++;
            }
            if (input[pos] == '*' && input[pos+1] == '/') {
                pos += 2;
            }
            continue;
        }
        
        // Not whitespace or comment
        break;
    }
    return pos;
}

/**
 * @brief Parse a PTX code block enclosed in curly braces
 * 
 * Parses a complete code block starting with an opening brace '{' and
 * ending with a closing brace '}', including all statements inside the block.
 *
 * @param input The input string to parse
 * @param consumed The number of characters consumed by parsing
 * @param result The resulting code block AST node
 * @return true if parsing succeeded, false otherwise
 */
bool ptx_parse_code_block(const char* input, size_t* consumed, ptx_code_block_t** result) {
    if (!input || !consumed || !result) {
        return false;
    }
    
    size_t pos = 0;
    
    // Skip initial whitespace and comments
    pos += skip_whitespace_and_comments(input + pos);
    
    // Check for opening brace
    if (input[pos] != '{') {
        return false;
    }
    pos++; // Move past the opening brace
    
    // Initialize the code block
    *result = ptx_code_block_init();
    if (!*result) {
        return false; // Memory allocation failed
    }
    
    // Parse statements until we reach the closing brace
    while (input[pos]) {
        // Skip whitespace and comments
        pos += skip_whitespace_and_comments(input + pos);
        
        // Check for closing brace
        if (input[pos] == '}') {
            pos++; // Move past the closing brace
            *consumed = pos;
            return true; // Successfully parsed the entire code block
        }
        
        // Parse a single statement
        struct ptx_statement_t* statement = NULL;
        
        // Find where this statement ends (either at a semicolon or at the next line)
        const char* stmt_start = input + pos;
        const char* stmt_end = stmt_start;
        int brace_level = 0;
        
        while (*stmt_end) {
            if (*stmt_end == '{') {
                brace_level++;
            } else if (*stmt_end == '}') {
                if (brace_level == 0) {
                    // We've reached the end of the code block
                    break;
                }
                brace_level--;
            } else if (*stmt_end == ';' && brace_level == 0) {
                // End of statement
                stmt_end++;
                break;
            }
            stmt_end++;
        }
        
        // Copy the statement text
        size_t stmt_len = stmt_end - stmt_start;
        if (stmt_len == 0) {
            // Empty statement, just continue
            pos++;
            continue;
        }
        
        // Trim trailing whitespace from the statement
        const char* trim_end = stmt_end - 1;
        while (trim_end > stmt_start && isspace(*trim_end)) {
            trim_end--;
        }
        size_t trimmed_len = (trim_end - stmt_start) + 1;
        
        char* stmt_text = (char*)malloc(trimmed_len + 1);
        if (!stmt_text) {
            ptx_code_block_free(*result);
            *result = NULL;
            return false; // Memory allocation failed
        }
        
        // Copy the statement text, normalizing whitespace
        size_t j = 0;
        bool last_was_space = false;
        for (size_t i = 0; i < trimmed_len; i++) {
            char c = stmt_start[i];
            if (isspace(c)) {
                if (!last_was_space) {
                    // Replace any whitespace with a single space
                    stmt_text[j++] = ' ';
                    last_was_space = true;
                }
            } else {
                stmt_text[j++] = c;
                last_was_space = false;
            }
        }
        stmt_text[j] = '\0';
        
        // Parse the statement
        bool success = parse_statement(stmt_text, &statement);
        free(stmt_text); // Free the temporary text buffer
        
        if (success && statement) {
            // Add the statement to the code block
            if (!ptx_code_block_add_statement(*result, statement)) {
                ptx_code_block_free(*result);
                *result = NULL;
                return false; // Failed to add statement
            }
        } else {
            // Skip this statement if we couldn't parse it
            // In a more robust implementation, we might want to report an error here
        }
        
        // Move position past this statement
        pos += stmt_len;
    }
    
    // If we reach here without finding a closing brace, parsing failed
    ptx_code_block_free(*result);
    *result = NULL;
    return false;
} 