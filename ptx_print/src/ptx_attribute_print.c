#include <ptx_print/ptx_attribute_print.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

int print_attribute_tag_to_buffer(char* buffer, size_t buffer_size, ptx_attribute_tag_t tag) {
    if (!buffer || buffer_size == 0) {
        return -1;
    }
    
    int printed = 0;
    switch (tag) {
        case PTX_ATTR_MANAGED:
            printed = snprintf(buffer, buffer_size, ".managed");
            break;
        case PTX_ATTR_UNIFIED:
            printed = snprintf(buffer, buffer_size, ".unified");
            break;
        default:
            return -1; // Unknown tag
    }
    
    return (printed >= 0 && (size_t)printed < buffer_size) ? printed : -1;
}

int print_attribute_to_buffer(char* buffer, size_t buffer_size, const ptx_attribute_t* attr) {
    if (!buffer || buffer_size == 0 || !attr) {
        return -1;
    }
    
    int printed = snprintf(buffer, buffer_size, ".attribute ");
    if (printed < 0 || (size_t)printed >= buffer_size) {
        return -1;
    }
    
    int tag_printed = print_attribute_tag_to_buffer(buffer + printed, buffer_size - printed, attr->tag);
    if (tag_printed < 0) {
        return -1;
    }
    printed += tag_printed;
    
    // Add parameters for unified attribute
    if (attr->tag == PTX_ATTR_UNIFIED) {
        int params_printed = snprintf(buffer + printed, buffer_size - printed, 
                                    "(%" PRIu64 ", %" PRIu64 ")", 
                                    attr->unified.uuid1, attr->unified.uuid2);
        if (params_printed < 0 || (size_t)params_printed >= buffer_size - printed) {
            return -1;
        }
        printed += params_printed;
    }
    
    return printed;
} 