#include <ptx_parse/ptx_statement_parse.h>
#include <ptx_parse/ptx_label_parse.h>
#include <ptx_parse/ptx_directive_parse.h>
#include <ptx_parse/ptx_instruction_parse.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/**
 * @brief Parse a PTX statement string and construct a ptx_statement_t
 * 
 * This function parses a complete PTX statement, which may include an optional label
 * followed by either a directive or an instruction.
 *
 * @param str The string to parse as a PTX statement
 * @param statement Pointer to ptx_statement_t* where the result will be stored
 * @return true if the string was successfully parsed as a statement, false otherwise
 */
bool parse_statement(const char* str, struct ptx_statement_t** statement) {
    if (!str || !statement) {
        return false;
    }
    
    // Skip leading whitespace
    const char* current = str;
    while (*current && isspace(*current)) {
        current++;
    }
    
    // Check if we have anything to parse
    if (!*current) {
        return false;
    }
    
    // Allocate memory for the statement
    *statement = (struct ptx_statement_t*)malloc(sizeof(struct ptx_statement_t));
    if (!*statement) {
        return false; // Memory allocation failed
    }
    
    // Initialize statement with null values (especially for the label)
    (*statement)->label.name = NULL;
    
    // Try to parse a label first (it's optional)
    struct ptx_label_t* label = NULL;
    bool has_label = parse_label(current, &label);
    
    if (has_label) {
        // We found a label, store it in the statement
        (*statement)->label = *label;
        
        // Move current pointer past the label (to the colon)
        current = strstr(current, label->name) + strlen(label->name);
        
        // Skip the colon and any whitespace after it
        while (*current && (*current == ':' || isspace(*current))) {
            current++;
        }
        
        // Free the label structure as we've copied its contents
        free(label);
    }
    
    // Now try to parse a directive
    if (*current == '.') {
        current++; // Skip the period
        
        // Try to extract the directive name
        const char* directive_start = current;
        while (*current && !isspace(*current) && *current != '(') {
            current++;
        }
        
        // Create a temporary buffer for the directive name
        size_t directive_length = current - directive_start;
        char* directive_name = (char*)malloc(directive_length + 1);
        if (!directive_name) {
            free(*statement);
            *statement = NULL;
            return false; // Memory allocation failed
        }
        
        strncpy(directive_name, directive_start, directive_length);
        directive_name[directive_length] = '\0';
        
        // Parse the directive
        ptx_directive_t directive;
        bool directive_parsed = parse_directive(directive_name, &directive);
        free(directive_name); // We don't need this anymore
        
        if (directive_parsed) {
            (*statement)->tag = DIRECTIVE;
            (*statement)->directive = directive;
            return true;
        } else {
            // Failed to parse the directive
            if (has_label) {
                free((*statement)->label.name);
            }
            free(*statement);
            *statement = NULL;
            return false;
        }
    } else {
        // Try to parse an instruction
        // First, extract just the instruction name (up to first space, period, or semicolon)
        const char* instr_start = current;
        while (*current && !isspace(*current) && *current != '.' && *current != ';') {
            current++;
        }
        
        // Create a temporary buffer for the instruction name
        size_t instr_length = current - instr_start;
        char* instr_name = (char*)malloc(instr_length + 1);
        if (!instr_name) {
            free(*statement);
            *statement = NULL;
            return false; // Memory allocation failed
        }
        
        strncpy(instr_name, instr_start, instr_length);
        instr_name[instr_length] = '\0';
        
        // Parse the instruction
        ptx_instruction_t instruction;
        bool instruction_parsed = parse_instruction(instr_name, &instruction);
        free(instr_name); // We don't need this anymore
        
        if (instruction_parsed) {
            (*statement)->tag = INSTRUCTION;
            (*statement)->instruction = instruction;
            return true;
        } else {
            // Failed to parse the instruction
            if (has_label) {
                free((*statement)->label.name);
            }
            free(*statement);
            *statement = NULL;
            return false;
        }
    }
} 