#include <stdlib.h>
#include <string.h>
#include "ptx_ast/ptx_function.h"

/**
 * Create a new function parameter
 */
ptx_parameter_t* ptx_parameter_create(ptx_type_t type, const char* name) {
    ptx_parameter_t* param = (ptx_parameter_t*)malloc(sizeof(ptx_parameter_t));
    if (!param) {
        return NULL;
    }
    
    param->type = type;
    param->name = strdup(name);
    if (!param->name) {
        free(param);
        return NULL;
    }
    
    param->state_space = 0; // No state space
    param->attribute = PTX_PARAM_NONE;
    param->has_alignment = false;
    param->alignment = 4; // Default alignment is 4 bytes
    param->next = NULL;
    
    return param;
}

/**
 * Free a function parameter
 */
void ptx_parameter_free(ptx_parameter_t* param) {
    if (!param) {
        return;
    }
    
    ptx_parameter_t* current = param;
    while (current) {
        ptx_parameter_t* next = current->next;
        free(current->name);
        
        // Free array shape if present
        if (current->has_array_dims && current->array_shape) {
            ptx_array_shape_free(current->array_shape);
        }
        
        free(current);
        current = next;
    }
}

/**
 * Create a new function
 */
ptx_function_t* ptx_function_create(ptx_function_directive_t directive, const char* name) {
    ptx_function_t* function = (ptx_function_t*)malloc(sizeof(ptx_function_t));
    if (!function) {
        return NULL;
    }
    
    memset(function, 0, sizeof(ptx_function_t));
    function->directive = directive;
    function->name = strdup(name);
    if (!function->name) {
        free(function);
        return NULL;
    }
    
    return function;
}

/**
 * Free a function
 */
void ptx_function_free(ptx_function_t* function) {
    if (!function) {
        return;
    }
    
    free(function->name);
    ptx_parameter_free(function->parameters);
    // Note: ptx_statement_free(function->body) would be necessary
    // when statements are implemented
    
    free(function);
} 