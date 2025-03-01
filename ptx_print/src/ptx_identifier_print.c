#include <ptx_print/ptx_identifier_print.h>
#include <string.h>

/**
 * Helper function to convert a predefined identifier to its string representation
 */
static const char* get_predefined_identifier_name(predefined_identifier_t id) {
    switch (id) {
        case CLOCK: return "%clock";
        case LANEID: return "%laneid";
        case LANEMASK_GT: return "%lanemask_gt";
        case PM0: return "%pm0";
        case PM1: return "%pm1";
        case PM2: return "%pm2";
        case PM3: return "%pm3";
        case PM4: return "%pm4";
        case PM5: return "%pm5";
        case PM6: return "%pm6";
        case PM7: return "%pm7";
        case CLOCK64: return "%clock64";
        case LANEMASK_EQ: return "%lanemask_eq";
        case NCTAID: return "%nctaid";
        case SMID: return "%smid";
        case CTAID: return "%ctaid";
        case LANEMASK_LE: return "%lanemask_le";
        case NTID: return "%ntid";
        case TID: return "%tid";
        case ENVREG32: return "%envreg32";
        case LANEMASK_LT: return "%lanemask_lt";
        case NSMID: return "%nsmid";
        case WARPID: return "%warpid";
        case GRIDID: return "%gridid";
        case LANEMASK_GE: return "%lanemask_ge";
        case NWARPID: return "%nwarpid";
        case WARP_SZ: return "WARP_SZ";
        default: return "<unknown-predefined-identifier>";
    }
}

// Prints an identifier to a file stream
bool print_identifier_to_file(FILE* file, ptx_identifier_t identifier) {
    if (!file) {
        return false;
    }
    
    int chars_written = 0;
    
    if (identifier.tag == PTX_IDENTIFIER_PREDEFINED) {
        const char* name = get_predefined_identifier_name(identifier.predefined);
        chars_written = fprintf(file, "%s", name);
    } else if (identifier.tag == PTX_IDENTIFIER_USER_DEFINED) {
        if (identifier.user_defined) {
            chars_written = fprintf(file, "%s", identifier.user_defined);
        } else {
            return false;
        }
    } else {
        return false;
    }
    
    return (chars_written > 0);
}

// Prints an identifier to a string buffer
int print_identifier_to_buffer(char* buffer, size_t buffer_size, ptx_identifier_t identifier) {
    if (!buffer || buffer_size == 0) {
        return -1;
    }
    
    int chars_written = 0;
    
    if (identifier.tag == PTX_IDENTIFIER_PREDEFINED) {
        const char* name = get_predefined_identifier_name(identifier.predefined);
        chars_written = snprintf(buffer, buffer_size, "%s", name);
    } else if (identifier.tag == PTX_IDENTIFIER_USER_DEFINED) {
        if (identifier.user_defined) {
            chars_written = snprintf(buffer, buffer_size, "%s", identifier.user_defined);
        } else {
            return -1;
        }
    } else {
        return -1;
    }
    
    return chars_written;
} 