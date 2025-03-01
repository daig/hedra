#include <ptx_parse/ptx_declaration_lhs_parse.h>
#include <ptx_parse/ptx_statespace_parse.h>
#include <ptx_parse/ptx_type_parse.h>
#include <prelude/ptx_array_shape.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>  // For printf debugging

/**
 * Helper function to skip whitespace
 */
static const char* skip_whitespace(const char* str) {
    while (str && *str && isspace(*str)) {
        str++;
    }
    return str;
}

/**
 * Helper function to parse a vector size specifier (.v2 or .v4)
 */
static bool parse_vector_size(const char** str_ptr, ptx_vector_size_t* vector_size) {
    const char* str = *str_ptr;
    
    // Skip whitespace
    str = skip_whitespace(str);
    
    // Check for .v2 or .v4
    if (strncmp(str, ".v2", 3) == 0) {
        *vector_size = VECTOR_SIZE_V2;
        *str_ptr = str + 3;
        return true;
    } else if (strncmp(str, ".v4", 3) == 0) {
        *vector_size = VECTOR_SIZE_V4;
        *str_ptr = str + 3;
        return true;
    }
    
    return false;
}

/**
 * Helper function to parse an identifier (variable name)
 */
static bool parse_identifier(const char** str_ptr, char** name) {
    const char* str = *str_ptr;
    
    // Skip whitespace
    str = skip_whitespace(str);
    
    // First character must be a letter or underscore
    if (!isalpha(*str) && *str != '_') {
        return false;
    }
    
    // Find end of identifier
    const char* start = str;
    while (isalnum(*str) || *str == '_' || *str == '$') {
        str++;
    }
    
    // Allocate and copy the identifier
    size_t len = str - start;
    *name = malloc(len + 1);
    if (!*name) {
        return false;
    }
    
    memcpy(*name, start, len);
    (*name)[len] = '\0';
    
    *str_ptr = str;
    return true;
}

/**
 * Helper function to parse array dimensions
 */
static bool parse_array_dimensions(const char** str_ptr, ptx_array_shape_t* array_shape) {
    const char* str = *str_ptr;
    
    // Count dimensions
    int num_dimensions = 0;
    const char* counter = str;
    
    while (*counter) {
        counter = skip_whitespace(counter);
        if (*counter != '[') {
            break;
        }
        
        num_dimensions++;
        counter++;
        
        // Skip to closing bracket
        while (*counter && *counter != ']') {
            counter++;
        }
        if (*counter != ']') {
            return false; // Missing closing bracket
        }
        counter++;
    }
    
    if (num_dimensions == 0) {
        return false; // No dimensions found
    }
    
    // Allocate array for dimensions (plus one for null terminator)
    size_t* dimensions = malloc((num_dimensions + 1) * sizeof(size_t));
    if (!dimensions) {
        return false;
    }
    
    // Parse dimensions
    for (int i = 0; i < num_dimensions; i++) {
        str = skip_whitespace(str);
        if (*str != '[') {
            free(dimensions);
            return false;
        }
        str++;
        
        // Parse dimension size
        char* endptr;
        dimensions[i] = strtoul(str, &endptr, 10);
        
        if (endptr == str || *endptr != ']') {
            free(dimensions);
            return false;
        }
        
        str = endptr + 1; // Skip past the closing bracket
    }
    
    // Add null terminator dimension
    dimensions[num_dimensions] = 0;
    
    *array_shape = dimensions;
    *str_ptr = str;
    return true;
}

/**
 * Helper function to parse alignment specifier (.align byte-count)
 */
static bool parse_alignment(const char** str_ptr, unsigned int* alignment) {
    const char* str = *str_ptr;
    
    // Skip whitespace
    str = skip_whitespace(str);
    
    // Check for .align
    if (strncmp(str, ".align", 6) != 0) {
        return false;
    }
    
    str += 6;
    str = skip_whitespace(str);
    
    // Parse the alignment value
    char* endptr;
    unsigned long align_val = strtoul(str, &endptr, 10);
    
    if (endptr == str) {
        return false; // No digits found
    }
    
    // Check if alignment is a power of 2
    if (align_val == 0 || (align_val & (align_val - 1)) != 0) {
        return false; // Not a power of 2
    }
    
    *alignment = (unsigned int)align_val;
    *str_ptr = endptr;
    return true;
}

/**
 * Internal version of parse_state_space that returns the updated position in the string
 */
static bool internal_parse_state_space(const char* str, ptx_state_space_t* space, const char** end_ptr) {
    if (!str || !space) {
        return false;
    }
    
    // Skip whitespace
    str = skip_whitespace(str);
    
    // All state spaces start with a period
    if (*str != '.') {
        return false;
    }
    
    // Check for each state space
    if (strncmp(str, ".reg", 4) == 0) {
        *space = PTX_STATE_REG;
        *end_ptr = str + 4;
        return true;
    } else if (strncmp(str, ".sreg", 5) == 0) {
        *space = PTX_STATE_SREG;
        *end_ptr = str + 5;
        return true;
    } else if (strncmp(str, ".const", 6) == 0) {
        *space = PTX_STATE_CONST;
        *end_ptr = str + 6;
        return true;
    } else if (strncmp(str, ".global", 7) == 0) {
        *space = PTX_STATE_GLOBAL;
        *end_ptr = str + 7;
        return true;
    } else if (strncmp(str, ".local", 6) == 0) {
        *space = PTX_STATE_LOCAL;
        *end_ptr = str + 6;
        return true;
    } else if (strncmp(str, ".param", 6) == 0) {
        *space = PTX_STATE_PARAM;
        *end_ptr = str + 6;
        return true;
    } else if (strncmp(str, ".shared", 7) == 0) {
        *space = PTX_STATE_SHARED;
        *end_ptr = str + 7;
        return true;
    } else if (strncmp(str, ".tex", 4) == 0) {
        *space = PTX_STATE_TEX;
        *end_ptr = str + 4;
        return true;
    }
    
    return false;
}

/**
 * Internal version of parse_type that returns the updated position in the string
 */
static bool internal_parse_type(const char* str, ptx_type_t* type, const char** end_ptr) {
    // Skip whitespace
    str = skip_whitespace(str);
    
    // Use the parse_type_with_position function with position tracking
    if (!parse_type_with_position(str, type, end_ptr)) {
        return false;
    }
    
    return true;
}

bool parse_declaration_lhs(const char* str, ptx_declaration_type_t* lhs) {
    if (!str || !lhs) {
        return false;
    }
    
    // Initialize the lhs structure
    memset(lhs, 0, sizeof(ptx_declaration_type_t));
    lhs->shape.kind = SHAPE_SCALAR; // Default to scalar
    
    // Make a copy of the original string for parsing
    const char* current = str;
    
    // Parse state space and get the updated position
    const char* state_space_end = NULL;
    if (!internal_parse_state_space(current, &lhs->statespace, &state_space_end)) {
        return false;
    }
    
    // Skip past the state space part
    current = state_space_end;
    
    // Skip whitespace after state space
    current = skip_whitespace(current);
    
    if (!*current) {
        return false; // Unexpected end of string
    }
    
    // Check for alignment specifier (.align)
    if (strncmp(current, ".align", 6) == 0) {
        if (!parse_alignment(&current, &lhs->alignment)) {
            return false;
        }
        
        // Skip whitespace after alignment
        current = skip_whitespace(current);
        
        if (!*current) {
            return false; // Unexpected end of string
        }
    }
    
    // Check for vector size specifier (.v2 or .v4)
    ptx_vector_size_t vector_size;
    if (parse_vector_size(&current, &vector_size)) {
        lhs->shape.kind = SHAPE_VECTOR;
        lhs->shape.vector_size = vector_size;
        
        // Skip whitespace after vector size
        current = skip_whitespace(current);
        
        if (!*current) {
            return false; // Unexpected end of string
        }
    }
    
    // Parse type and get the updated position
    const char* type_end = NULL;
    if (!internal_parse_type(current, &lhs->type, &type_end)) {
        return false;
    }
    
    // Skip past the type part
    current = type_end;
    
    // Skip whitespace after type
    current = skip_whitespace(current);
    
    if (!*current) {
        return false; // Unexpected end of string
    }
    
    // Parse identifier (variable name)
    if (!parse_identifier(&current, &lhs->name)) {
        return false;
    }
    
    // Check for array dimensions
    current = skip_whitespace(current);
    
    if (*current == '[') {
        if (lhs->shape.kind == SHAPE_VECTOR) {
            // Can't have both vector and array
            free(lhs->name);
            lhs->name = NULL;
            return false;
        }
        
        ptx_array_shape_t array_shape = NULL;
        if (!parse_array_dimensions(&current, &array_shape)) {
            free(lhs->name);
            lhs->name = NULL;
            return false;
        }
        
        lhs->shape.kind = SHAPE_ARRAY;
        lhs->shape.array_shape = array_shape;
    }
    
    // We're not handling initializer parsing in this function
    lhs->has_initializer = false;
    
    return true;
} 