#include <ptx_ast/>ptx_identifier.h>
#include <ctype.h>

bool is_followsym(char c) {
    return isalnum(c) || c == '_' || c == '$';
}

bool is_valid_user_defined_identifier(const char* str) {
    if (!str || !*str) { return false; }

    // First character must be letter, underscore, dollar, or percent
    if (isalpha(*str)) {
        // If starts with letter, rest can be followsym
        str++;
        while (*str) {
            if (!is_followsym(*str)) { return false; }
            str++; 
        }
        return true;
    } else if (*str == '_' || *str == '$' || *str == '%') {
        // If starts with _, $, or %, must have at least one more char
        str++;
        if (!*str) { return false; }
        // Rest must be followsym
        while (*str) {
            if (!is_followsym(*str)) { return false; }
            str++; 
        }
        return true;
    }
    
    return false;
} 