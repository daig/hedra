#include <ptx_parse/ptx_type_decl_parse.h>
#include <ptx_parse/ptx_declaration_lhs_parse.h>
#include <ptx_parse/ptx_initializer_value_parse.h>
#include <ptx_parse/ptx_initializer_array_parse.h>
#include <prelude/ptx_array_shape.h>
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
 * Helper function to check for an equals sign followed by an initializer
 */
static bool check_for_initializer(const char* str, int* pos) {
    // Save the current position
    int saved_pos = *pos;
    
    // Skip whitespace
    skip_whitespace(str, pos);
    
    // Check for equals sign
    if (str[*pos] != '=') {
        // No initializer found, restore position
        *pos = saved_pos;
        return false;
    }
    
    // Skip the equals sign
    (*pos)++;
    
    return true;
}

/**
 * Helper function to free resources in an initializer value
 */
static void free_initializer_value(ptx_initializer_value_t* value) {
    if (!value) {
        return;
    }
    
    if (value->kind == INIT_VALUE_SCALAR && value->scalar_expr) {
        free_expr(value->scalar_expr);
        value->scalar_expr = NULL;
    } else if (value->kind == INIT_VALUE_ADDR_VAR && value->addr_var.var_name) {
        free((void*)value->addr_var.var_name);
        value->addr_var.var_name = NULL;
    }
}

void free_ptx_decl(ptx_decl_t* decl) {
    if (!decl) {
        return;
    }
    
    // Free the name in the declaration LHS
    if (decl->type.name) {
        free(decl->type.name);
        decl->type.name = NULL;
    }
    
    // Free array shape if present
    if (decl->type.shape.kind == SHAPE_ARRAY && decl->type.shape.array_shape) {
        ptx_array_shape_free(decl->type.shape.array_shape);
        decl->type.shape.array_shape = NULL;
    }
    
    // No need to free attribute data as it doesn't contain any dynamically allocated memory
    
    // Free initializer if present
    if (decl->type.has_initializer) {
        if ((decl->type.shape.kind == SHAPE_ARRAY || decl->type.shape.kind == SHAPE_VECTOR) && decl->array) {
            
            size_t total_elements = 0;
            if (decl->type.shape.kind == SHAPE_ARRAY && decl->type.shape.array_shape) {
                // Get the total number of elements in the array
                total_elements = ptx_array_shape_elements(decl->type.shape.array_shape);
            } else if (decl->type.shape.kind == SHAPE_VECTOR) {
                // For vectors, the total elements is the vector size (2 or 4)
                total_elements = (size_t)decl->type.shape.vector_size;
            }
            
            // Create indices array for getting elements
            size_t ndims = 1; // For vectors, we treat as 1D array
            if (decl->type.shape.kind == SHAPE_ARRAY) {
                ndims = ptx_array_shape_ndims(decl->type.shape.array_shape);
            }
            
            // Free each initializer value in the array
            for (size_t i = 0; i < total_elements; i++) {
                // We need to compute multi-dimensional indices from the linear index
                size_t* indices = (size_t*)malloc(ndims * sizeof(size_t));
                
                if (indices) {
                    if (decl->type.shape.kind == SHAPE_ARRAY) {
                        // Compute indices from linear index for multi-dimensional arrays
                        size_t linear_index = i;
                        for (size_t dim = ndims; dim > 0; dim--) {
                            size_t dim_size = decl->type.shape.array_shape[dim - 1];
                            indices[dim - 1] = linear_index % dim_size;
                            linear_index /= dim_size;
                        }
                    } else {
                        // For vectors, just use the index directly as it's 1D
                        indices[0] = i;
                    }
                    
                    // Get the value pointer and free its resources
                    ptx_initializer_value_t* value_ptr = 
                        (ptx_initializer_value_t*)ptx_initializer_array_get(decl->array, indices);
                    
                    if (value_ptr) {
                        free_initializer_value(value_ptr);
                    }
                    
                    free(indices);
                }
            }
            
            // Free the array itself
            ptx_initializer_array_free(decl->array);
            decl->array = NULL;
        } else if (decl->scalar) {
            // Free scalar initializer
            free_initializer_value(decl->scalar);
            free(decl->scalar);
            decl->scalar = NULL;
        }
    }
}

bool parse_ptx_decl(const char* str, ptx_decl_t* decl) {
    if (!str || !decl) {
        return false;
    }
    
    // Initialize the declaration structure
    memset(decl, 0, sizeof(ptx_decl_t));
    
    // Parse the declaration left-hand side
    if (!parse_declaration_lhs(str, &decl->type)) {
        return false;
    }
    
    // Find the position after the LHS in the original string
    int pos = 0;
    const char* current = str;
    
    // Skip whitespace at the beginning
    while (isspace(*current)) {
        current++;
        pos++;
    }
    
    // Skip the declaration LHS part
    // This is a bit tricky since we don't know exactly where the LHS ends in the original string.
    // We'll look for the name and any array dimensions.
    const char* name_pos = strstr(current, decl->type.name);
    if (!name_pos) {
        return false;  // Name not found in string
    }
    
    // Move to the position after the name
    pos += (name_pos - current) + strlen(decl->type.name);
    
    // If it's an array, we need to skip the dimensions too
    if (decl->type.shape.kind == SHAPE_ARRAY) {
        // Skip each dimension [N][M]...
        size_t ndims = ptx_array_shape_ndims(decl->type.shape.array_shape);
        for (size_t i = 0; i < ndims; i++) {
            // Find the opening bracket
            while (str[pos] != '[' && str[pos] != '\0') {
                pos++;
            }
            if (str[pos] == '\0') {
                return false;  // Unexpected end of string
            }
            
            // Skip to the closing bracket
            while (str[pos] != ']' && str[pos] != '\0') {
                pos++;
            }
            if (str[pos] == '\0') {
                return false;  // Unexpected end of string
            }
            
            // Skip the closing bracket
            pos++;
        }
    }
    
    // Check if there's an initializer
    if (check_for_initializer(str, &pos)) {
        decl->type.has_initializer = true;
        
        // Skip whitespace after the equals sign
        skip_whitespace(str, &pos);
        
        // Parse the initializer based on the shape of the declaration
        if (decl->type.shape.kind == SHAPE_ARRAY) {
            // Parse array initializer
            return parse_initializer_array(str, &pos, decl->type.shape.array_shape, &decl->array, decl->type.type);
        } else if (decl->type.shape.kind == SHAPE_VECTOR) {
            // For vector initializers, we'll create a temporary 1D array shape
            // with size equal to the vector size (2 or 4)
            size_t vector_size = (size_t)decl->type.shape.vector_size;
            ptx_array_shape_t temp_shape = ptx_array_shape_create(1, vector_size);
            
            if (!temp_shape) {
                return false;
            }
            
            // Parse the vector initializer as a 1D array
            bool result = parse_initializer_array(str, &pos, temp_shape, &decl->array, decl->type.type);
            
            // Free the temporary shape
            ptx_array_shape_free(temp_shape);
            
            return result;
        } else {
            // Parse scalar initializer
            decl->scalar = (ptx_initializer_value_t*)malloc(sizeof(ptx_initializer_value_t));
            if (!decl->scalar) {
                return false;
            }
            
            // Initialize to zero
            memset(decl->scalar, 0, sizeof(ptx_initializer_value_t));
            
            return parse_scalar_initializer(str, &pos, decl->scalar, decl->type.type);
        }
    } else {
        // No initializer
        decl->type.has_initializer = false;
    }
    
    return true;
} 