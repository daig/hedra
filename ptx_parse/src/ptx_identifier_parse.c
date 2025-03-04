#include <ptx_parse/ptx_identifier_parse.h>
#include <ptx_ast/ptx_identifier.h>
#include <ptx_ast/ptx_predefined_identifier.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

/**
 * @brief Parse a PTX identifier string and construct a ptx_identifier_t
 * 
 * @param str The string to parse as a potential identifier
 * @param identifier Pointer to ptx_identifier_t* where the result will be stored
 * @return true if the string is a valid identifier and was successfully parsed, false otherwise
 */
bool parse_identifier(const char* str, ptx_identifier_t** identifier) {
    if (!str || !identifier) {
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
    
    // Allocate memory for the identifier structure
    *identifier = (ptx_identifier_t*)malloc(sizeof(ptx_identifier_t));
    if (!*identifier) {
        return false; // Memory allocation failed
    }
    
    // Check if it's a predefined identifier (starts with %)
    if (*str == '%') {
        // Try to match it against known predefined identifiers
        predefined_identifier_t predefined_id = get_predefined_identifier(str);
        
        // If it's a valid predefined identifier
        if (predefined_id != -1) {  // Assuming -1 is the sentinel value for "not found"
            (*identifier)->tag = PTX_IDENTIFIER_PREDEFINED;
            (*identifier)->predefined = predefined_id;
            return true;
        } else {
            // Not a valid predefined identifier
            free(*identifier);
            *identifier = NULL;
            return false;
        }
    } 
    // Otherwise, it's a user-defined identifier
    else {
        // Extract just the identifier part (up to whitespace or special chars)
        size_t len = 0;
        const char* tmp = str;
        while (*tmp && !isspace(*tmp) && *tmp != '(' && *tmp != ',' && *tmp != ')') {
            len++;
            tmp++;
        }
        
        // Create a temporary null-terminated string for the identifier
        char* id_str = (char*)malloc(len + 1);
        if (!id_str) {
            free(*identifier);
            *identifier = NULL;
            return false;
        }
        
        strncpy(id_str, str, len);
        id_str[len] = '\0';
        
        // Now check if it's a valid user-defined identifier
        if (is_valid_user_defined_identifier(id_str)) {
            (*identifier)->tag = PTX_IDENTIFIER_USER_DEFINED;
            
            // Allocate memory for the identifier name
            (*identifier)->user_defined = strdup(id_str);
            if (!(*identifier)->user_defined) {
                free(id_str);
                free(*identifier);
                *identifier = NULL;
                return false; // Memory allocation failed
            }
            
            free(id_str);
            return true;
        }
        
        free(id_str);
        free(*identifier);
        *identifier = NULL;
        return false;
    }
} 