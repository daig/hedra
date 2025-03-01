#include <ptx_parse/ptx_initializer_array_parse.h>
#include <ptx_parse/ptx_initializer_value_parse.h>
#include <ptx_parse/ptx_constant_expr_parse.h>
#include <ptx_ast/ptx_constant_expr.h>
#include <ptx_ast/ptx_type_decl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/**
 * Helper function to skip whitespace in the input string
 */
static void skip_whitespace(const char* str, int* pos) {
    while (str[*pos] != '\0' && isspace(str[*pos])) {
        (*pos)++;
    }
}

/**
 * Helper function to create a deep copy of an expression
 */
static ptx_expr_t* deep_copy_expr(const ptx_expr_t* src) {
    if (!src) {
        return NULL;
    }
    
    switch (src->type) {
        case EXPR_CONSTANT:
            return create_constant_expr(src->constant);
            
        case EXPR_UNARY_OP: {
            ptx_expr_t* operand_copy = deep_copy_expr(src->unary.operand);
            if (!operand_copy) {
                return NULL;
            }
            return create_unary_expr(src->unary.op, operand_copy);
        }
            
        case EXPR_BINARY_OP: {
            ptx_expr_t* left_copy = deep_copy_expr(src->binary.left);
            if (!left_copy) {
                return NULL;
            }
            
            ptx_expr_t* right_copy = deep_copy_expr(src->binary.right);
            if (!right_copy) {
                free_expr(left_copy);
                return NULL;
            }
            
            return create_binary_expr(src->binary.op, left_copy, right_copy);
        }
            
        case EXPR_TERNARY_OP: {
            ptx_expr_t* condition_copy = deep_copy_expr(src->ternary.condition);
            if (!condition_copy) {
                return NULL;
            }
            
            ptx_expr_t* true_copy = deep_copy_expr(src->ternary.true_expr);
            if (!true_copy) {
                free_expr(condition_copy);
                return NULL;
            }
            
            ptx_expr_t* false_copy = deep_copy_expr(src->ternary.false_expr);
            if (!false_copy) {
                free_expr(condition_copy);
                free_expr(true_copy);
                return NULL;
            }
            
            return create_ternary_expr(condition_copy, true_copy, false_copy);
        }
            
        default:
            return NULL;
    }
}

/**
 * Helper function to create a deep copy of a scalar initializer value
 * 
 * @param dest Destination initializer value
 * @param src Source initializer value
 * @return true if copy successful, false otherwise
 */
static bool deep_copy_initializer_value(ptx_initializer_value_t* dest, const ptx_initializer_value_t* src) {
    if (!dest || !src) {
        return false;
    }
    
    // Copy basic fields
    dest->kind = src->kind;
    dest->mask = src->mask;
    
    // Deep copy based on kind
    if (src->kind == INIT_VALUE_SCALAR) {
        if (src->scalar_expr) {
            dest->scalar_expr = deep_copy_expr(src->scalar_expr);
            if (!dest->scalar_expr) {
                return false;
            }
        } else {
            dest->scalar_expr = NULL;
        }
    } else if (src->kind == INIT_VALUE_ADDR_VAR) {
        if (src->addr_var.var_name) {
            dest->addr_var.var_name = strdup(src->addr_var.var_name);
            if (!dest->addr_var.var_name) {
                return false;
            }
        } else {
            dest->addr_var.var_name = NULL;
        }
    }
    
    return true;
}

/**
 * Helper function to set an initializer value in the array with proper deep copying
 */
static int set_initializer_value(ptx_initializer_array_t* array, const size_t* indices, const ptx_initializer_value_t* value) {
    if (!array || !indices || !value) {
        return -1;
    }
    
    // Get the destination pointer
    ptx_initializer_value_t* dest = (ptx_initializer_value_t*)ptx_initializer_array_get(array, indices);
    if (!dest) {
        return -1;
    }
    
    // Free any existing resources in the destination
    if (dest->kind == INIT_VALUE_SCALAR && dest->scalar_expr) {
        free_expr(dest->scalar_expr);
        dest->scalar_expr = NULL;
    } else if (dest->kind == INIT_VALUE_ADDR_VAR && dest->addr_var.var_name) {
        free((void*)dest->addr_var.var_name);
        dest->addr_var.var_name = NULL;
    }
    
    // Perform deep copy
    if (!deep_copy_initializer_value(dest, value)) {
        return -1;
    }
    
    return 0;
}

/**
 * Helper function to recursively parse a nested array initializer
 * 
 * @param str String to parse
 * @param pos Pointer to current position in string (will be updated)
 * @param array The array to fill
 * @param array_shape Shape of the array
 * @param dim_index Current dimension index (0 for outermost dimension)
 * @param current_indices Current indices array (will be modified during recursion)
 * @return true if parsing successful, false otherwise
 */
static bool parse_nested_array_initializer(
    const char* str, 
    int* pos, 
    ptx_initializer_array_t* array, 
    ptx_array_shape_t array_shape,
    size_t dim_index,
    size_t* current_indices
) {
    // Skip whitespace
    skip_whitespace(str, pos);
    
    // Get the number of dimensions
    size_t ndims = ptx_array_shape_ndims(array_shape);
    
    // Check if we've reached the end of dimensions
    if (dim_index >= ndims) {
        return false;
    }
    
    // Get the size of the current dimension
    size_t current_dim_size = array_shape[dim_index];
    
    // Check for opening brace
    if (str[*pos] != '{') {
        // If we're at the innermost dimension expecting a scalar, parse a single value
        if (dim_index == ndims - 1) {
            // Parse a single initializer value
            ptx_initializer_value_t value;
            memset(&value, 0, sizeof(value)); // Initialize to zero
            
            if (!parse_scalar_initializer(str, pos, &value)) {
                return false;
            }
            
            // Set it in the array using deep copy
            int result = set_initializer_value(array, current_indices, &value);
            
            // Free resources allocated for the original value (deep copy has been made)
            if (value.kind == INIT_VALUE_SCALAR && value.scalar_expr != NULL) {
                free_expr(value.scalar_expr);
            } else if (value.kind == INIT_VALUE_ADDR_VAR && value.addr_var.var_name != NULL) {
                free((void*)value.addr_var.var_name);
            }
            
            return (result == 0);
        }
        return false; // Missing opening brace for non-innermost dimension
    }
    
    // Skip past the opening brace
    (*pos)++;
    
    // Parse each element in this dimension
    size_t element_count = 0;
    for (size_t i = 0; i < current_dim_size; i++) {
        // Update the current indices for this element
        current_indices[dim_index] = i;
        
        // Skip whitespace
        skip_whitespace(str, pos);
        
        // Check for premature closing brace (too few elements)
        if (str[*pos] == '}') {
            if (i < current_dim_size - 1) {
                // Too few elements
                return false;
            }
            break;
        }
        
        // Check if we're at the innermost dimension
        if (dim_index == ndims - 1) {
            // Parse a single value for this element
            ptx_initializer_value_t value;
            memset(&value, 0, sizeof(value)); // Initialize to zero
            
            if (!parse_scalar_initializer(str, pos, &value)) {
                return false;
            }
            
            // Set the value in the array using deep copy
            int result = set_initializer_value(array, current_indices, &value);
            
            // Free resources allocated for the original value (deep copy has been made)
            if (value.kind == INIT_VALUE_SCALAR && value.scalar_expr != NULL) {
                free_expr(value.scalar_expr);
            } else if (value.kind == INIT_VALUE_ADDR_VAR && value.addr_var.var_name != NULL) {
                free((void*)value.addr_var.var_name);
            }
            
            if (result != 0) {
                return false;
            }
            
            element_count++;
        } else {
            // For non-innermost dimensions, we need to handle the next level of nesting
            // Recursive call to handle the next dimension
            if (!parse_nested_array_initializer(str, pos, array, array_shape, dim_index + 1, current_indices)) {
                return false;
            }
            
            element_count++;
        }
        
        // Skip whitespace after the value
        skip_whitespace(str, pos);
        
        // Check for comma separator if not the last element
        if (i < current_dim_size - 1) {
            if (str[*pos] != ',') {
                return false;
            }
            (*pos)++;
            
            // Check for trailing comma (comma followed by closing brace)
            skip_whitespace(str, pos);
            if (str[*pos] == '}') {
                return false;  // Trailing comma is not allowed
            }
        }
    }
    
    // Check for too many elements
    skip_whitespace(str, pos);
    if (str[*pos] == ',') {
        // There's a comma after the expected number of elements
        (*pos)++;
        skip_whitespace(str, pos);
        
        // If there's another element after the comma, it's too many elements
        if (str[*pos] != '}') {
            return false;
        }
    }
    
    // Skip whitespace before the closing brace
    skip_whitespace(str, pos);
    
    // Check for closing brace
    if (str[*pos] != '}') {
        return false;
    }
    
    // Skip past the closing brace
    (*pos)++;
    
    // Ensure we have the correct number of elements
    if (element_count != current_dim_size) {
        return false;
    }
    
    return true;
}

bool parse_initializer_array(const char* str, int* pos, ptx_array_shape_t array_shape, ptx_initializer_array_t** array) {
    if (!str || !pos || !array_shape || !array) {
        return false;
    }
    
    // Skip whitespace
    skip_whitespace(str, pos);
    
    // Create a new array to hold the initializer values
    *array = ptx_initializer_array_create(array_shape, sizeof(ptx_initializer_value_t));
    if (!*array) {
        return false;
    }
    
    // Initialize all array elements with default values
    size_t total_elements = ptx_array_shape_elements(array_shape);
    for (size_t i = 0; i < total_elements; i++) {
        // We'll need to manually compute indices from the linear index
        // since ptx_array_shape_linear_to_indices doesn't exist
        size_t linear_index = i;
        size_t ndims = ptx_array_shape_ndims(array_shape);
        size_t* indices = (size_t*)malloc(ndims * sizeof(size_t));
        
        if (indices) {
            // Compute multi-dimensional indices
            for (size_t dim = ndims; dim > 0; dim--) {
                size_t dim_size = array_shape[dim - 1];
                indices[dim - 1] = linear_index % dim_size;
                linear_index /= dim_size;
            }
            
            // Get the element pointer
            ptx_initializer_value_t* value_ptr = (ptx_initializer_value_t*)ptx_initializer_array_get(*array, indices);
            if (value_ptr) {
                // Initialize with zeros
                memset(value_ptr, 0, sizeof(ptx_initializer_value_t));
                value_ptr->kind = INIT_VALUE_SCALAR;
                value_ptr->mask = MASK_UNMASKED;
                value_ptr->scalar_expr = NULL;
            }
            free(indices);
        }
    }
    
    // Allocate an array to track current indices during parsing
    size_t ndims = ptx_array_shape_ndims(array_shape);
    size_t* current_indices = (size_t*)calloc(ndims, sizeof(size_t));
    if (!current_indices) {
        ptx_initializer_array_free(*array);
        *array = NULL;
        return false;
    }
    
    // Parse the nested array initializer
    bool success = parse_nested_array_initializer(str, pos, *array, array_shape, 0, current_indices);
    
    // Free the current indices array
    free(current_indices);
    
    // If parsing failed, free the array and return false
    if (!success) {
        ptx_initializer_array_free(*array);
        *array = NULL;
        return false;
    }
    
    return true;
} 