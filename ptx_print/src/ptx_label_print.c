#include <ptx_print/ptx_label_print.h>
#include <string.h>

bool print_label_to_file(FILE* file, const ptx_label_t* label) {
    if (!file || !label || !label->name) {
        return false;
    }
    
    // Print the label name followed by a colon as per PTX syntax
    fprintf(file, "%s:", label->name);
    return true;
}

int print_label_to_buffer(char* buffer, size_t buffer_size, const ptx_label_t* label) {
    if (!buffer || buffer_size == 0 || !label || !label->name) {
        return -1;
    }
    
    // Print the label name followed by a colon as per PTX syntax
    return snprintf(buffer, buffer_size, "%s:", label->name);
} 