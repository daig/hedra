#include <ptx_print/ptx_constant_print.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Prints a constant value to a file stream
bool print_constant_to_file(FILE* file, ptx_constant_t constant) {
    if (!file) {
        return false;
    }
    
    int chars_written = 0;
    
    switch (constant.type) {
        case PTX_CONST_INT_SIGNED:
            chars_written = fprintf(file, "%lld", (long long)constant.s64_val);
            break;
        case PTX_CONST_INT_UNSIGNED:
            chars_written = fprintf(file, "%lluU", (unsigned long long)constant.u64_val);
            break;
        case PTX_CONST_FLOAT:
            chars_written = fprintf(file, "%.6f", constant.f64_val);
            break;
        case PTX_CONST_FLOAT_SINGLE:
            chars_written = fprintf(file, "%.6f", (double)constant.f32_val);
            break;
        case PTX_CONST_PRED:
            chars_written = fprintf(file, "%d", constant.pred_val);
            break;
        default:
            return false;
    }
    
    return (chars_written > 0);
}

// Prints a constant value to a string buffer
int print_constant_to_buffer(char* buffer, size_t buffer_size, ptx_constant_t constant) {
    if (!buffer || buffer_size == 0) {
        return -1;
    }
    
    int chars_written = 0;
    
    switch (constant.type) {
        case PTX_CONST_INT_SIGNED:
            chars_written = snprintf(buffer, buffer_size, "%lld", (long long)constant.s64_val);
            break;
        case PTX_CONST_INT_UNSIGNED:
            chars_written = snprintf(buffer, buffer_size, "%lluU", (unsigned long long)constant.u64_val);
            break;
        case PTX_CONST_FLOAT:
            chars_written = snprintf(buffer, buffer_size, "%.6f", constant.f64_val);
            break;
        case PTX_CONST_FLOAT_SINGLE:
            chars_written = snprintf(buffer, buffer_size, "%.6f", (double)constant.f32_val);
            break;
        case PTX_CONST_PRED:
            chars_written = snprintf(buffer, buffer_size, "%d", constant.pred_val);
            break;
        default:
            return -1;
    }
    
    return chars_written;
} 