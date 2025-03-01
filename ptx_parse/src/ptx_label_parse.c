#include <ptx_parse/ptx_label_parse.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/**
 * @brief Checks if a character is valid for starting a PTX identifier
 * 
 * Valid first characters for PTX identifiers are letters, underscore, dollar sign, or percent sign.
 *
 * @param c The character to check
 * @return true if valid, false otherwise
 */
static bool is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_' || c == '$' || c == '%';
}

/**
 * @brief Checks if a character is valid for a PTX identifier (non-first position)
 * 
 * Valid characters for PTX identifiers (after the first) are letters, digits, underscores, or dollar signs.
 *
 * @param c The character to check
 * @return true if valid, false otherwise
 */
static bool is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_' || c == '$';
}

/**
 * @brief Parse a PTX label string and construct a ptx_label_t
 * 
 * @param str The string to parse as a potential label
 * @param label Pointer to ptx_label_t* where the result will be stored
 * @return true if the string is a valid label and was successfully parsed, false otherwise
 */
bool parse_label(const char* str, ptx_label_t** label) {
    if (!str || !label) {
        return false;
    }
    
    // Skip leading whitespace
    while (*str && isspace(*str)) {
        str++;
    }
    
    // Check if we have anything to parse
    if (!*str) {
        return false;
    }
    
    // Check if the first character is valid for an identifier
    if (!is_valid_identifier_start(*str)) {
        return false;
    }
    
    // Find the end of the identifier
    const char* identifier_start = str;
    str++;
    while (*str && is_valid_identifier_char(*str)) {
        str++;
    }
    
    // Save the end of the identifier before skipping whitespace
    const char* identifier_end = str;
    
    // Check if we have a colon after the identifier (possibly with whitespace)
    while (*str && isspace(*str)) {
        str++;
    }
    
    if (*str != ':') {
        return false;
    }
    
    // We found a valid label
    size_t identifier_length = identifier_end - identifier_start;
    
    // Allocate memory for the label structure
    *label = (ptx_label_t*)malloc(sizeof(ptx_label_t));
    if (!*label) {
        return false; // Memory allocation failed
    }
    
    // Allocate memory for the label name and copy it
    (*label)->name = (char*)malloc(identifier_length + 1);
    if (!(*label)->name) {
        free(*label);
        *label = NULL;
        return false; // Memory allocation failed
    }
    
    // Copy the identifier (without the colon and any whitespace)
    strncpy((*label)->name, identifier_start, identifier_length);
    (*label)->name[identifier_length] = '\0';
    
    return true;
} 