#include <ptx_print/ptx_declaration_lhs_print.h>
#include <ptx_print/ptx_statespace_print.h>
#include <ptx_print/ptx_type_print.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Print a vector size as .v2 or .v4
 * 
 * @param file File to print to
 * @param vector_size Vector size to print
 * @return bool True if successful, false otherwise
 */
static bool print_vector_size_to_file(FILE* file, ptx_vector_size_t vector_size) {
    switch (vector_size) {
        case VECTOR_SIZE_V2:
            return fputs(".v2", file) >= 0;
        case VECTOR_SIZE_V4:
            return fputs(".v4", file) >= 0;
        default:
            return false;
    }
}

/**
 * @brief Print array dimensions to a file
 * 
 * @param file File to print to
 * @param shape Array shape to print
 * @return bool True if successful, false otherwise
 */
static bool print_array_dimensions_to_file(FILE* file, ptx_array_shape_t shape) {
    if (!shape) {
        return true;  // Nothing to print for NULL shape
    }

    size_t ndims = ptx_array_shape_ndims(shape);
    for (size_t i = 0; i < ndims; i++) {
        if (fprintf(file, "[%zu]", shape[i]) < 0) {
            return false;
        }
    }
    
    return true;
}

bool print_declaration_lhs_to_file(FILE* file, const ptx_declaration_type_t* lhs) {
    if (!file || !lhs) {
        return false;
    }

    // Print state space
    if (!print_state_space_to_file(file, lhs->statespace)) {
        return false;
    }

    // Print vector size (if applicable)
    if (lhs->shape.kind == SHAPE_VECTOR) {
        if (fputc(' ', file) < 0) {
            return false;
        }
        if (!print_vector_size_to_file(file, lhs->shape.vector_size)) {
            return false;
        }
    }

    // Print type
    if (fputc(' ', file) < 0) {
        return false;
    }
    if (!print_type_to_file(file, lhs->type)) {
        return false;
    }

    // Print variable name
    if (lhs->name) {
        if (fprintf(file, " %s", lhs->name) < 0) {
            return false;
        }
    }

    // Print array dimensions (if applicable)
    if (lhs->shape.kind == SHAPE_ARRAY) {
        if (!print_array_dimensions_to_file(file, lhs->shape.array_shape)) {
            return false;
        }
    }

    return true;
}

int print_declaration_lhs_to_buffer(char* buffer, size_t buffer_size, const ptx_declaration_type_t* lhs) {
    if (!buffer || buffer_size == 0 || !lhs) {
        return -1;
    }

    // Use a temporary file backed by a dynamic buffer
    char* temp_buffer = NULL;
    size_t temp_size = 0;
    FILE* memfile = open_memstream(&temp_buffer, &temp_size);
    if (!memfile) {
        return -1;
    }

    bool success = print_declaration_lhs_to_file(memfile, lhs);
    fclose(memfile);

    if (!success || !temp_buffer) {
        free(temp_buffer);
        return -1;
    }

    // Copy from temp buffer to the provided buffer
    size_t len = strlen(temp_buffer);
    if (len >= buffer_size) {
        len = buffer_size - 1;  // Leave room for null terminator
    }
    memcpy(buffer, temp_buffer, len);
    buffer[len] = '\0';  // Ensure null termination
    
    free(temp_buffer);
    return len;
} 