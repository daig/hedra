#include <ptx_print/ptx_directive_print.h>
#include <string.h>

// Struct to map directive enum values to strings
// This is the inverse of the mapping in ptx_directive_parse.c
typedef struct {
    const char* name;
    ptx_directive_t directive;
} directive_map_entry;

// Array of directive mappings
static const directive_map_entry directive_map[] = {
    {"address_size", PTX_DIRECTIVE_ADDRESS_SIZE},
    {"explicitcluster", PTX_DIRECTIVE_EXPLICITCLUSTER},
    {"maxnreg", PTX_DIRECTIVE_MAXNREG},
    {"section", PTX_DIRECTIVE_SECTION},
    {"alias", PTX_DIRECTIVE_ALIAS},
    {"extern", PTX_DIRECTIVE_EXTERN},
    {"maxntid", PTX_DIRECTIVE_MAXNTID},
    {"shared", PTX_DIRECTIVE_SHARED},
    {"align", PTX_DIRECTIVE_ALIGN},
    {"file", PTX_DIRECTIVE_FILE},
    {"minnctapersm", PTX_DIRECTIVE_MINNCTAPERSM},
    {"sreg", PTX_DIRECTIVE_SREG},
    {"branchtargets", PTX_DIRECTIVE_BRANCHTARGETS},
    {"func", PTX_DIRECTIVE_FUNC},
    {"noreturn", PTX_DIRECTIVE_NORETURN},
    {"target", PTX_DIRECTIVE_TARGET},
    {"callprototype", PTX_DIRECTIVE_CALLPROTOTYPE},
    {"global", PTX_DIRECTIVE_GLOBAL},
    {"param", PTX_DIRECTIVE_PARAM},
    {"tex", PTX_DIRECTIVE_TEX},
    {"calltargets", PTX_DIRECTIVE_CALLTARGETS},
    {"loc", PTX_DIRECTIVE_LOC},
    {"pragma", PTX_DIRECTIVE_PRAGMA},
    {"version", PTX_DIRECTIVE_VERSION},
    {"common", PTX_DIRECTIVE_COMMON},
    {"local", PTX_DIRECTIVE_LOCAL},
    {"reg", PTX_DIRECTIVE_REG},
    {"visible", PTX_DIRECTIVE_VISIBLE},
    {"const", PTX_DIRECTIVE_CONST},
    {"maxclusterrank", PTX_DIRECTIVE_MAXCLUSTERRANK},
    {"reqnctapercluster", PTX_DIRECTIVE_REQNCTAPERCLUSTER},
    {"weak", PTX_DIRECTIVE_WEAK},
    {"entry", PTX_DIRECTIVE_ENTRY},
    {"maxnctapersm", PTX_DIRECTIVE_MAXNCTAPERSM},
    {"reqntid", PTX_DIRECTIVE_REQNTID}
};

static const size_t directive_map_size = sizeof(directive_map) / sizeof(directive_map[0]);

const char* get_directive_string(ptx_directive_t directive) {
    for (size_t i = 0; i < directive_map_size; i++) {
        if (directive_map[i].directive == directive) {
            return directive_map[i].name;
        }
    }
    return NULL;  // Directive not found
}

bool print_directive_to_file(FILE* file, ptx_directive_t directive) {
    if (!file) {
        return false;
    }
    
    const char* directive_str = get_directive_string(directive);
    if (!directive_str) {
        return false;
    }
    
    // Print the directive with a leading dot as per PTX syntax
    fprintf(file, ".%s", directive_str);
    return true;
}

int print_directive_to_buffer(char* buffer, size_t buffer_size, ptx_directive_t directive) {
    if (!buffer || buffer_size == 0) {
        return -1;
    }
    
    const char* directive_str = get_directive_string(directive);
    if (!directive_str) {
        return -1;
    }
    
    // Print the directive with a leading dot as per PTX syntax
    return snprintf(buffer, buffer_size, ".%s", directive_str);
} 