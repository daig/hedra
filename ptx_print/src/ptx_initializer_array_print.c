#include <ptx_print/ptx_initializer_array_print.h>
#include <ptx_print/ptx_initializer_value_print.h>
#include <ptx_ast/ptx_type_decl.h>
#include <string.h>
#include <stdlib.h>

/**
 * Helper function to recursively print a nested array.
 * 
 * @param file The file to print to.
 * @param array The initializer array.
 * @param array_shape The array shape.
 * @param current_indices Current indices in the recursion.
 * @param dim_index Current dimension index (0 for outermost dimension).
 * @return true if successful, false if an error occurs.
 */
static bool print_nested_array(
    FILE* file,
    const ptx_initializer_array_t* array,
    ptx_array_shape_t array_shape,
    size_t* current_indices,
    size_t dim_index
) {
    // Get the number of dimensions
    size_t ndims = ptx_array_shape_ndims(array_shape);
    
    // Check if we've reached the end of dimensions
    if (dim_index >= ndims) {
        return false;
    }
    
    // Get the size of the current dimension
    size_t current_dim_size = array_shape[dim_index];
    
    // Print opening brace
    if (fprintf(file, "{") < 0) {
        return false;
    }
    
    // Iterate through elements in this dimension
    for (size_t i = 0; i < current_dim_size; i++) {
        // Update the current indices for this element
        current_indices[dim_index] = i;
        
        // Check if we're at the innermost dimension
        if (dim_index == ndims - 1) {
            // Get the value at the current indices
            ptx_initializer_value_t* value = (ptx_initializer_value_t*)ptx_initializer_array_get(
                (ptx_initializer_array_t*)array,
                current_indices
            );
            
            // Print the value
            if (value) {
                if (print_ptx_initializer_value(file, value) < 0) {
                    return false;
                }
            } else {
                // Default to 0 if value not found
                if (fprintf(file, "0") < 0) {
                    return false;
                }
            }
        } else {
            // For non-innermost dimensions, we need to handle the next level of nesting
            // Recursive call to handle the next dimension
            if (!print_nested_array(file, array, array_shape, current_indices, dim_index + 1)) {
                return false;
            }
        }
        
        // Print comma if not the last element
        if (i < current_dim_size - 1) {
            if (fprintf(file, ", ") < 0) {
                return false;
            }
        }
    }
    
    // Print closing brace
    if (fprintf(file, "}") < 0) {
        return false;
    }
    
    return true;
}

bool print_ptx_initializer_array_to_file(FILE* file, const ptx_initializer_array_t* array) {
    if (!file || !array) {
        return false;
    }
    
    // Get the array shape
    ptx_array_shape_t array_shape = ptx_initializer_array_shape(array);
    if (!array_shape) {
        return false;
    }
    
    // Allocate an array to track current indices during printing
    size_t ndims = ptx_array_shape_ndims(array_shape);
    size_t* current_indices = (size_t*)calloc(ndims, sizeof(size_t));
    if (!current_indices) {
        return false;
    }
    
    // Print the nested array
    bool success = print_nested_array(file, array, array_shape, current_indices, 0);
    
    // Free the current indices array
    free(current_indices);
    
    return success;
}

int sprint_ptx_initializer_array(char* str, size_t size, const ptx_initializer_array_t* array) {
    if (!str || size == 0 || !array) {
        return -1;
    }
    
    // Create a temporary file in memory
    FILE* mem_file = fmemopen(str, size, "w");
    if (!mem_file) {
        return -1;
    }
    
    // Print to the memory file
    bool success = print_ptx_initializer_array_to_file(mem_file, array);
    
    // Get the number of bytes written
    int bytes_written = ftell(mem_file);
    
    // Close the memory file
    fclose(mem_file);
    
    // Ensure null termination
    if (success && bytes_written >= 0 && (size_t)bytes_written < size) {
        str[bytes_written] = '\0';
        return bytes_written;
    } 
    
    // Handle errors
    if (!success) {
        return -1;
    }
    
    // Handle truncation
    if ((size_t)bytes_written >= size && size > 0) {
        str[size - 1] = '\0';
    }
    
    return bytes_written;
} 