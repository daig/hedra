#include <ptx_print/ptx_constant_print.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Helper function to print IEEE 754 single-precision float in hex format
static int print_f32_ieee_hex(FILE* file, float value) {
    uint32_t bits;
    memcpy(&bits, &value, sizeof(float));
    return fprintf(file, "0F%08X", bits);
}

// Helper function to print IEEE 754 double-precision float in hex format
static int print_f64_ieee_hex(FILE* file, double value) {
    uint64_t bits;
    memcpy(&bits, &value, sizeof(double));
    return fprintf(file, "0D%016llX", (unsigned long long)bits);
}

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
        case PTX_CONST_FLOAT_SCIENTIFIC:
            // Use the original string if available, otherwise use scientific notation
            if (constant.original_str) {
                chars_written = fprintf(file, "%s", constant.original_str);
            } else {
                chars_written = fprintf(file, "%.6e", constant.f64_val);
            }
            break;
        case PTX_CONST_FLOAT_IEEE_HEX_F32:
            // Use the original string if available, otherwise generate IEEE hex format
            if (constant.original_str) {
                chars_written = fprintf(file, "%s", constant.original_str);
            } else {
                chars_written = print_f32_ieee_hex(file, constant.f32_val);
            }
            break;
        case PTX_CONST_FLOAT_IEEE_HEX_F64:
            // Use the original string if available, otherwise generate IEEE hex format
            if (constant.original_str) {
                chars_written = fprintf(file, "%s", constant.original_str);
            } else {
                chars_written = print_f64_ieee_hex(file, constant.f64_val);
            }
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
        case PTX_CONST_FLOAT_SCIENTIFIC:
            // Use the original string if available, otherwise use scientific notation
            if (constant.original_str) {
                chars_written = snprintf(buffer, buffer_size, "%s", constant.original_str);
            } else {
                chars_written = snprintf(buffer, buffer_size, "%.6e", constant.f64_val);
            }
            break;
        case PTX_CONST_FLOAT_IEEE_HEX_F32:
            // Use the original string if available, otherwise generate IEEE hex format
            if (constant.original_str) {
                chars_written = snprintf(buffer, buffer_size, "%s", constant.original_str);
            } else {
                uint32_t bits;
                memcpy(&bits, &constant.f32_val, sizeof(float));
                chars_written = snprintf(buffer, buffer_size, "0F%08X", bits);
            }
            break;
        case PTX_CONST_FLOAT_IEEE_HEX_F64:
            // Use the original string if available, otherwise generate IEEE hex format
            if (constant.original_str) {
                chars_written = snprintf(buffer, buffer_size, "%s", constant.original_str);
            } else {
                uint64_t bits;
                memcpy(&bits, &constant.f64_val, sizeof(double));
                chars_written = snprintf(buffer, buffer_size, "0D%016llX", (unsigned long long)bits);
            }
            break;
        case PTX_CONST_PRED:
            chars_written = snprintf(buffer, buffer_size, "%d", constant.pred_val);
            break;
        default:
            return -1;
    }
    
    return chars_written;
} 