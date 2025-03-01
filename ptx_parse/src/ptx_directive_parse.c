#include <ptx_parse/ptx_directive_parse.h>
#include <string.h>
#include <ctype.h>

// Struct to map directive strings to enum values
typedef struct {
    const char* name;
    ptx_directive_t directive;
} directive_map_entry;

// Array of directive mappings
// The strings should be in lowercase for case-insensitive comparison
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

// Number of entries in the directive_map array
static const int directive_map_size = sizeof(directive_map) / sizeof(directive_map_entry);

/**
 * Case-insensitive string comparison
 * @param s1 First string
 * @param s2 Second string
 * @return 0 if strings are equal (case-insensitive), non-zero otherwise
 */
static int str_case_cmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        int c1 = tolower((unsigned char)*s1);
        int c2 = tolower((unsigned char)*s2);
        if (c1 != c2) {
            return c1 - c2;
        }
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

/**
 * Parse a PTX directive string and store in ptx_directive_t
 * 
 * This function takes a string representing a PTX directive (e.g., "version", "target", "func"),
 * and determines the corresponding directive type from the ptx_directive_t enum.
 *
 * @param str The directive string to parse (without the leading period)
 * @param directive Pointer to ptx_directive_t where the result will be stored
 * @return true if successful, false if the directive is not recognized
 */
bool parse_directive(const char* str, ptx_directive_t* directive) {
    if (!str || !directive) {
        return false;
    }

    // Perform a linear search through the directive map
    for (int i = 0; i < directive_map_size; i++) {
        if (str_case_cmp(str, directive_map[i].name) == 0) {
            *directive = directive_map[i].directive;
            return true;
        }
    }

    // Directive not found
    return false;
} 