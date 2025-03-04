#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "prelude/prelude.h"
#include "ptx_parse/ptx_function_parse.h"
#include "ptx_parse/ptx_directive_parse.h"
#include "ptx_parse/ptx_identifier_parse.h"
#include "ptx_parse/ptx_type_parse.h"
#include "ptx_parse/ptx_attribute_parse.h"
#include "ptx_parse/ptx_statespace_parse.h"

// Helper function to parse whitespace and comments
static size_t skip_whitespace_and_comments(const char* input) {
    size_t pos = 0;
    while (input[pos] && (isspace(input[pos]) || input[pos] == '/')) {
        if (input[pos] == '/' && input[pos+1] == '/') {
            // Skip single line comment
            pos += 2;
            while (input[pos] && input[pos] != '\n') {
                pos++;
            }
            if (input[pos] == '\n') {
                pos++;
            }
        } else if (isspace(input[pos])) {
            pos++;
        } else {
            break;
        }
    }
    return pos;
}

/**
 * Parse a function directive (.entry or .func)
 */
static bool parse_function_directive(const char* input, size_t* consumed, ptx_function_directive_t* directive) {
    size_t pos = skip_whitespace_and_comments(input);
    
    // Check for .entry
    if (strncmp(input + pos, ".entry", 6) == 0 && (isspace(input[pos+6]) || input[pos+6] == 0)) {
        *directive = PTX_FUNC_ENTRY;
        *consumed = pos + 6;
        return true;
    }
    
    // Check for .func
    if (strncmp(input + pos, ".func", 5) == 0 && (isspace(input[pos+5]) || input[pos+5] == 0)) {
        *directive = PTX_FUNC_FUNC;
        *consumed = pos + 5;
        return true;
    }
    
    return false;
}

/**
 * Parse a function attribute (.visible or .extern)
 */
static bool parse_function_attribute(const char* input, size_t* consumed, ptx_function_t* function) {
    size_t pos = skip_whitespace_and_comments(input);
    
    // Check for .visible
    if (strncmp(input + pos, ".visible", 8) == 0 && (isspace(input[pos+8]) || input[pos+8] == 0)) {
        function->is_visible = true;
        *consumed = pos + 8;
        return true;
    }
    
    // Check for .extern
    if (strncmp(input + pos, ".extern", 7) == 0 && (isspace(input[pos+7]) || input[pos+7] == 0)) {
        function->is_extern = true;
        *consumed = pos + 7;
        return true;
    }
    
    return false;
}

/**
 * Parse a parameter with potential .ptr attribute from PTX
 */
static bool parse_parameter(const char* input, size_t* consumed, ptx_parameter_t* param) {
    size_t pos = 0;
    
    // Initialize parameter
    param->next = NULL;
    param->name = NULL;
    param->attribute = PTX_PARAM_NONE;
    param->pointer_state_space = PTX_STATE_GLOBAL; // Default to global if not specified
    param->has_alignment = false;
    param->alignment = 4; // Default alignment is 4 bytes
    param->has_array_dims = false;
    param->array_shape = NULL;
    
    // Check for state space directive (.param or .reg)
    if (strncmp(input + pos, ".param", 6) == 0 && (isspace(input[pos+6]) || input[pos+6] == 0)) {
        param->state_space = PTX_STATE_PARAM;
        pos += 6;
        pos += skip_whitespace_and_comments(input + pos);
    } else if (strncmp(input + pos, ".reg", 4) == 0 && (isspace(input[pos+4]) || input[pos+4] == 0)) {
        param->state_space = PTX_STATE_REG;
        pos += 4;
        pos += skip_whitespace_and_comments(input + pos);
    } else {
        return false;
    }
    
    // Check for alignment (can appear before or after type)
    if (strncmp(input + pos, ".align", 6) == 0 && (isspace(input[pos+6]) || input[pos+6] == 0)) {
        pos += 6;
        pos += skip_whitespace_and_comments(input + pos);
        
        // Parse alignment value
        char* endptr;
        long alignment = strtol(input + pos, &endptr, 10);
        if (endptr == input + pos || alignment <= 0) {
            return false;
        }
        
        param->has_alignment = true;
        param->alignment = (unsigned int)alignment;
        pos = endptr - input;
        pos += skip_whitespace_and_comments(input + pos);
    }
    
    // Parse type
    ptx_type_t type;
    const char* end_ptr;
    if (!parse_type_with_position(input + pos, &type, &end_ptr)) {
        return false;
    }
    param->type = type;
    pos = end_ptr - input;
    pos += skip_whitespace_and_comments(input + pos);
    
    // Check for .ptr attribute (only valid for .param state space)
    if (param->state_space == PTX_STATE_PARAM && 
        strncmp(input + pos, ".ptr", 4) == 0 && 
        (isspace(input[pos+4]) || input[pos+4] == '.' || input[pos+4] == 0)) {
        
        param->attribute = PTX_PARAM_PTR;
        pos += 4;
        pos += skip_whitespace_and_comments(input + pos);
        
        // Check for state space after .ptr
        if (input[pos] == '.') {
            // Extract the state space token
            const char* space_start = input + pos;
            const char* space_end = space_start;
            
            // Skip the initial dot
            space_end++;
            
            // Find the end of the state space token (up to the next period or whitespace)
            while (*space_end && *space_end != '.' && !isspace(*space_end)) {
                space_end++;
            }
            
            // Create a temporary string for the state space
            size_t space_len = space_end - space_start;
            char* space_str = (char*)malloc(space_len + 1);
            if (!space_str) {
                return false;
            }
            strncpy(space_str, space_start, space_len);
            space_str[space_len] = '\0';
            
            // Parse the state space
            ptx_state_space_t ptr_state_space;
            if (parse_state_space(space_str, &ptr_state_space)) {
                // Store the pointer's target state space in our new field
                param->pointer_state_space = ptr_state_space;
                pos += space_len;
                pos += skip_whitespace_and_comments(input + pos);
            } else {
                free(space_str);
                return false;
            }
            
            free(space_str);
        }
        
        // Check for alignment after .ptr if not already set
        if (!param->has_alignment && strncmp(input + pos, ".align", 6) == 0 && (isspace(input[pos+6]) || input[pos+6] == 0)) {
            pos += 6;
            pos += skip_whitespace_and_comments(input + pos);
            
            // Parse alignment value
            char* endptr;
            long alignment = strtol(input + pos, &endptr, 10);
            if (endptr == input + pos || alignment <= 0) {
                return false;
            }
            
            param->has_alignment = true;
            param->alignment = (unsigned int)alignment;
            pos = endptr - input;
            pos += skip_whitespace_and_comments(input + pos);
        }
    }
    
    // Parse parameter name
    ptx_identifier_t* identifier = NULL;
    
    // Find the end of the identifier (up to whitespace, comma, closing parenthesis, or opening bracket)
    const char* id_start = input + pos;
    const char* id_end = id_start;
    
    // Skip leading whitespace
    while (*id_start && isspace(*id_start)) {
        id_start++;
    }
    
    // Find the end of the identifier
    id_end = id_start;
    while (*id_end && !isspace(*id_end) && *id_end != ',' && *id_end != ')' && *id_end != '[') {
        id_end++;
    }
    
    // Create a temporary string for just the identifier
    size_t id_len = id_end - id_start;
    if (id_len == 0) {
        return false;
    }
    
    char* id_str = malloc(id_len + 1);
    if (!id_str) {
        return false;
    }
    
    strncpy(id_str, id_start, id_len);
    id_str[id_len] = '\0';
    
    // Parse the identifier
    if (!parse_identifier(id_str, &identifier)) {
        free(id_str);
        return false;
    }
    
    free(id_str);
    
    // Copy the identifier - user_defined field contains the name
    if (identifier->tag == PTX_IDENTIFIER_USER_DEFINED) {
        param->name = strdup(identifier->user_defined);
    } else {
        // For predefined identifiers, convert the enum to a string
        char predefined_str[32];
        sprintf(predefined_str, "pred_%d", identifier->predefined);
        param->name = strdup(predefined_str);
    }
    
    // Update position to after the identifier
    pos = id_end - input;
    
    free(identifier);
    
    if (!param->name) {
        return false;
    }
    
    // Check for array dimensions
    if (input[pos] == '[') {
        // Parse array dimensions
        size_t dim_start = pos;
        
        // Count dimensions
        int num_dimensions = 0;
        const char* counter = input + pos;
        
        while (*counter) {
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
            
            // Skip whitespace between dimensions
            while (*counter && isspace(*counter)) {
                counter++;
            }
        }
        
        if (num_dimensions == 0) {
            // No dimensions found
            *consumed = pos;
            return true;
        }
        
        // Allocate array for dimensions (plus one for null terminator)
        size_t* dimensions = malloc((num_dimensions + 1) * sizeof(size_t));
        if (!dimensions) {
            return false;
        }
        
        // Parse dimensions
        counter = input + pos;
        for (int i = 0; i < num_dimensions; i++) {
            if (*counter != '[') {
                free(dimensions);
                return false;
            }
            counter++;
            
            // Parse dimension size
            char* endptr;
            dimensions[i] = strtoul(counter, &endptr, 10);
            
            if (endptr == counter || *endptr != ']') {
                free(dimensions);
                return false;
            }
            
            counter = endptr + 1; // Skip past the closing bracket
            
            // Skip whitespace between dimensions
            while (*counter && isspace(*counter)) {
                counter++;
            }
        }
        
        // Add null terminator dimension
        dimensions[num_dimensions] = 0;
        
        // Update position to the end of the array dimensions
        pos = counter - input;
        
        // Store array dimensions in parameter
        param->has_array_dims = true;
        param->array_shape = dimensions;
    }
    
    *consumed = pos;
    return true;
}

/**
 * Parse a parameter list from a PTX function declaration
 */
bool ptx_parse_parameter_list(const char* input, size_t* consumed, ptx_parameter_t** result) {
    size_t pos = skip_whitespace_and_comments(input);
    
    // Check for opening parenthesis
    if (input[pos] != '(') {
        return false;
    }
    pos++;
    pos += skip_whitespace_and_comments(input + pos);
    
    // Handle empty parameter list
    if (input[pos] == ')') {
        *result = NULL;
        *consumed = pos + 1; // +1 for the closing parenthesis
        return true;
    }
    
    ptx_parameter_t* first = NULL;
    ptx_parameter_t* current = NULL;
    
    // Parse comma-separated list of parameters
    while (true) {
        // Parse a parameter
        ptx_parameter_t* param = malloc(sizeof(ptx_parameter_t));
        if (!param) {
            // Free previously allocated parameters
            ptx_parameter_t* temp = first;
            while (temp) {
                ptx_parameter_t* next = temp->next;
                free(temp->name);
                free(temp);
                temp = next;
            }
            return false;
        }
        
        size_t param_consumed = 0;
        if (!parse_parameter(input + pos, &param_consumed, param)) {
            free(param);
            // Free previously allocated parameters
            ptx_parameter_t* temp = first;
            while (temp) {
                ptx_parameter_t* next = temp->next;
                free(temp->name);
                free(temp);
                temp = next;
            }
            return false;
        }
        
        // Add parameter to the list
        if (!first) {
            first = param;
            current = param;
        } else {
            current->next = param;
            current = param;
        }
        
        pos += param_consumed;
        pos += skip_whitespace_and_comments(input + pos);
        
        // Check for comma or closing parenthesis
        if (input[pos] == ',') {
            pos++;
            pos += skip_whitespace_and_comments(input + pos);
        } else if (input[pos] == ')') {
            pos++;
            break;
        } else {
            // Free allocated parameters
            ptx_parameter_t* temp = first;
            while (temp) {
                ptx_parameter_t* next = temp->next;
                free(temp->name);
                free(temp);
                temp = next;
            }
            return false;
        }
    }
    
    *result = first;
    *consumed = pos;
    return true;
}

/**
 * Parse a complete function declaration from PTX
 */
bool ptx_parse_function_declaration(const char* input, size_t* consumed, ptx_function_t* result) {
    size_t pos = 0;
    
    // Initialize the function structure
    memset(result, 0, sizeof(ptx_function_t));
    
    // Parse optional function attributes (.visible)
    size_t attr_consumed = 0;
    while (parse_function_attribute(input + pos, &attr_consumed, result)) {
        pos += attr_consumed;
        pos += skip_whitespace_and_comments(input + pos);
    }
    
    // Parse function directive (.entry or .func)
    size_t dir_consumed = 0;
    if (!parse_function_directive(input + pos, &dir_consumed, &result->directive)) {
        return false;
    }
    pos += dir_consumed;
    pos += skip_whitespace_and_comments(input + pos);
    
    // For device functions (.func), parse return parameter list first
    if (result->directive == PTX_FUNC_FUNC && input[pos] == '(') {
        size_t param_consumed = 0;
        if (!ptx_parse_parameter_list(input + pos, &param_consumed, &result->return_parameters)) {
            return false;
        }
        pos += param_consumed;
        pos += skip_whitespace_and_comments(input + pos);
    }
    
    // Parse function name
    ptx_identifier_t* identifier = NULL;
    if (!parse_identifier(input + pos, &identifier)) {
        // Free return parameters if they were allocated
        ptx_parameter_t* current_param = result->return_parameters;
        while (current_param) {
            ptx_parameter_t* next = current_param->next;
            free(current_param->name);
            free(current_param);
            current_param = next;
        }
        return false;
    }
    
    // Find end of identifier
    const char* id_start = input + pos;
    while (*id_start && !isspace(*id_start) && *id_start != '(' && *id_start != '{') {
        id_start++;
    }
    pos = id_start - input;
    
    // Copy the function name - user_defined field contains the name
    if (identifier->tag == PTX_IDENTIFIER_USER_DEFINED) {
        result->name = strdup(identifier->user_defined);
    } else {
        // For predefined identifiers, convert the enum to a string
        char predefined_str[32];
        sprintf(predefined_str, "pred_%d", identifier->predefined);
        result->name = strdup(predefined_str);
    }
    
    free(identifier);
    
    if (!result->name) {
        // Free return parameters if they were allocated
        ptx_parameter_t* current_param = result->return_parameters;
        while (current_param) {
            ptx_parameter_t* next = current_param->next;
            free(current_param->name);
            free(current_param);
            current_param = next;
        }
        return false;
    }
    
    pos += skip_whitespace_and_comments(input + pos);
    
    // Parse input parameter list
    if (input[pos] == '(') {
        size_t param_consumed = 0;
        if (!ptx_parse_parameter_list(input + pos, &param_consumed, &result->parameters)) {
            free(result->name);
            // Free return parameters if they were allocated
            ptx_parameter_t* current_param = result->return_parameters;
            while (current_param) {
                ptx_parameter_t* next = current_param->next;
                free(current_param->name);
                free(current_param);
                current_param = next;
            }
            return false;
        }
        pos += param_consumed;
    }
    
    *consumed = pos;
    return true;
} 