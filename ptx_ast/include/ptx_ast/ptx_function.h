/**
 * @file ptx_function.h
 * @brief Definitions for PTX function AST components
 */

#ifndef PTX_FUNCTION_H
#define PTX_FUNCTION_H

#include <stdbool.h>
#include <stdint.h>
#include "ptx_ast/ptx_identifier.h"
#include "ptx_ast/ptx_type.h"
#include "ptx_ast/ptx_declaration_lhs.h"
#include "ptx_ast/ptx_statement.h"
#include "prelude/ptx_array_shape.h"

/**
 * Function directive types in PTX
 */
typedef enum ptx_function_directive {
    PTX_FUNC_ENTRY,      // .entry - kernel entry point
    PTX_FUNC_FUNC,       // .func - device function
    PTX_FUNC_VISIBLE,    // .visible - externally visible
    PTX_FUNC_EXTERN      // .extern - external function
} ptx_function_directive_t;

/**
 * Parameter attribute type for function parameters
 */
typedef enum ptx_param_attribute {
    PTX_PARAM_NONE,        // No special attribute
    PTX_PARAM_PTR          // .ptr attribute - parameter is a pointer
} ptx_param_attribute_t;

/**
 * Structure representing a PTX function parameter
 */
typedef struct ptx_parameter {
    ptx_state_space_t state_space;   // State space (.const, .global, etc)
    ptx_type_t type;                 // Parameter type
    char* name;                      // Parameter name
    ptx_param_attribute_t attribute; // Parameter attribute (e.g., .ptr)
    ptx_state_space_t pointer_state_space; // For .ptr parameters, the target state space
    bool has_alignment;              // Whether alignment is specified
    unsigned int alignment;          // Alignment value in bytes (if specified)
    bool has_array_dims;             // Whether this parameter has array dimensions
    ptx_array_shape_t array_shape;   // Array dimensions (NULL if not an array)
    struct ptx_parameter* next;      // Linked list of parameters
} ptx_parameter_t;

/**
 * Structure representing a PTX function
 */
typedef struct ptx_function {
    ptx_function_directive_t directive;  // Function directive (.entry, .func)
    bool is_visible;                     // Whether function has .visible attribute
    bool is_extern;                      // Whether function is .extern
    char* name;                          // Function name
    ptx_parameter_t* return_parameters;  // Return parameters (for .func only)
    ptx_parameter_t* parameters;         // Input parameters
    ptx_statement_t* body;               // Function body statements
    bool has_attribute;                  // Whether this function has an attribute
    ptx_attribute_t attribute;           // Function attribute if present
} ptx_function_t;

#endif /* PTX_FUNCTION_H */ 