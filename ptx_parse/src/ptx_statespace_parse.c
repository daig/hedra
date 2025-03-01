#include <ptx_parse/ptx_statespace_parse.h>
#include <string.h>
#include <ctype.h>

// Struct to map state space strings to enum values
typedef struct {
    const char* name;
    ptx_state_space_t space;
} statespace_map_entry;

// Array of state space mappings
// The strings should be in lowercase for case-insensitive comparison
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

// Parse a string as a PTX state space
bool parse_state_space(const char* str, ptx_state_space_t* space) {
    if (!str || !space) {
        return false;
    }
    
    // State space strings start with a period
    if (str[0] != '.') {
        return false;
    }
    
    // Skip the period
    str++;
    
    // Compare with the known state space strings
    for (size_t i = 0; i < statespace_map_size; i++) {
        // Case-insensitive comparison
        if (strcasecmp(str, statespace_map[i].name) == 0) {
            *space = statespace_map[i].space;
            return true;
        }
    }
    
    // Not a recognized state space
    return false;
} 