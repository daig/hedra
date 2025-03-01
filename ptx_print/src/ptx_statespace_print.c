#include <ptx_print/ptx_statespace_print.h>
#include <stdio.h>
#include <string.h>

// Struct to map state space enum values to strings
typedef struct {
    const char* name;
    ptx_state_space_t space;
} statespace_map_entry;

// Array of state space mappings
static const statespace_map_entry statespace_map[] = {
    {"reg", PTX_STATE_REG},
    {"sreg", PTX_STATE_SREG},
    {"const", PTX_STATE_CONST},
    {"global", PTX_STATE_GLOBAL},
    {"local", PTX_STATE_LOCAL},
    {"param", PTX_STATE_PARAM},
    {"shared", PTX_STATE_SHARED},
    {"tex", PTX_STATE_TEX}
};

// Number of entries in the statespace_map array
static const size_t statespace_map_size = sizeof(statespace_map) / sizeof(statespace_map[0]);

// Get the string representation of a state space
static const char* get_state_space_name(ptx_state_space_t space) {
    for (size_t i = 0; i < statespace_map_size; i++) {
        if (statespace_map[i].space == space) {
            return statespace_map[i].name;
        }
    }
    return NULL; // Unknown state space
}

// Prints a state space to a file stream
bool print_state_space_to_file(FILE* file, ptx_state_space_t space) {
    if (!file) {
        return false;
    }
    
    const char* name = get_state_space_name(space);
    if (!name) {
        return false;
    }
    
    int chars_written = fprintf(file, ".%s", name);
    return (chars_written > 0);
}

// Prints a state space to a string buffer
int print_state_space_to_buffer(char* buffer, size_t buffer_size, ptx_state_space_t space) {
    if (!buffer || buffer_size == 0) {
        return -1;
    }
    
    const char* name = get_state_space_name(space);
    if (!name) {
        return -1;
    }
    
    return snprintf(buffer, buffer_size, ".%s", name);
} 