#include <ptx_print/c_directive_print.h>
#include <string.h>

/**
 * Convert a C directive enum to its string representation.
 */
static const char* c_directive_to_string(enum c_directive_t directive) {
    switch (directive) {
        case C_DIRECTIVE_INCLUDE:
            return "include";
        case C_DIRECTIVE_DEFINE:
            return "define";
        case C_DIRECTIVE_IF:
            return "if";
        case C_DIRECTIVE_IFDEF:
            return "ifdef";
        case C_DIRECTIVE_ELSE:
            return "else";
        case C_DIRECTIVE_ENDIF:
            return "endif";
        case C_DIRECTIVE_LINE:
            return "line";
        case C_DIRECTIVE_FILE:
            return "file";
        default:
            return "unknown";
    }
}

int print_c_directive(FILE* file, enum c_directive_t directive, const char* content) {
    const char* directive_str = c_directive_to_string(directive);
    
    // For directives that don't typically have content (like #else and #endif),
    // we still print them with a space but no content
    if (content == NULL || strlen(content) == 0) {
        return fprintf(file, "#%s", directive_str);
    }
    
    // For directives with content, print with a space after the directive name
    return fprintf(file, "#%s %s", directive_str, content);
}

int sprint_c_directive(char* str, size_t size, enum c_directive_t directive, const char* content) {
    const char* directive_str = c_directive_to_string(directive);
    
    // For directives that don't typically have content (like #else and #endif),
    // we still print them with a space but no content
    if (content == NULL || strlen(content) == 0) {
        return snprintf(str, size, "#%s", directive_str);
    }
    
    // For directives with content, print with a space after the directive name
    return snprintf(str, size, "#%s %s", directive_str, content);
} 