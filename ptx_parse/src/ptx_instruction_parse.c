#include <ptx_parse/ptx_instruction_parse.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Struct to map instruction strings to enum values
typedef struct {
    const char* name;
    ptx_instruction_tag instruction;
} instruction_map_entry;

// Array of instruction mappings
// The strings should be in lowercase for case-insensitive comparison
static const instruction_map_entry instruction_map[] = {
    {"abs", ABS},
    {"add", ADD},
    {"addc", ADDC},
    {"and", AND},
    {"atom", ATOM},
    {"bar", BAR},
    {"bfe", BFE},
    {"bfi", BFI},
    {"bfind", BFIND},
    {"bmsk", BMSK},
    {"bra", BRA},
    {"brev", BREV},
    {"brkpt", BRKPT},
    {"call", CALL},
    {"clz", CLZ},
    {"cnot", CNOT},
    {"copysign", COPYSIGN},
    {"cos", COS},
    {"cvt", CVT},
    {"cvta", CVTA},
    {"div", DIV},
    {"dp4a", DP4A},
    {"dp2a", DP2A},
    {"ex2", EX2},
    {"exit", EXIT},
    {"fma", FMA},
    {"isspacep", ISSPACEP},
    {"istypep", ISTYPEP},
    {"ld", LD},
    {"ldu", LDU},
    {"lg2", LG2},
    {"mad", MAD},
    {"mad24", MAD24},
    {"madc", MADC},
    {"max", MAX},
    {"membar", MEMBAR},
    {"min", MIN},
    {"mov", MOV},
    {"mul", MUL},
    {"mul24", MUL24},
    {"neg", NEG},
    {"not", NOT},
    {"or", OR},
    {"popc", POPC},
    {"prefetch", PREFETCH},
    {"prefetchu", PREFETCHU},
    {"prmt", PRMT},
    {"rcp", RCP},
    {"red", RED},
    {"rem", REM},
    {"ret", RET},
    {"rsqrt", RSQRT},
    {"sad", SAD},
    {"selp", SELP},
    {"set", SET},
    {"setp", SETP},
    {"shf", SHF},
    {"shfl", SHFL},
    {"shl", SHL},
    {"shr", SHR},
    {"sin", SIN},
    {"slct", SLCT},
    {"sqrt", SQRT},
    {"st", ST},
    {"sub", SUB},
    {"subc", SUBC},
    {"suld", SULD},
    {"suq", SUQ},
    {"sured", SURED},
    {"sust", SUST},
    {"testp", TESTP},
    {"tex", TEX},
    {"tld4", TLD4},
    {"trap", TRAP},
    {"txq", TXQ},
    {"vabsdiff", VABSDIFF},
    {"vadd", VADD},
    {"vote", VOTE},
    {"xor", XOR},
    // Add other instructions as needed from ptx_instruction.h enum
};

// Number of entries in the instruction_map array
static const int instruction_map_size = sizeof(instruction_map) / sizeof(instruction_map_entry);

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
 * Parse a PTX instruction name string and store in ptx_instruction_tag
 */
bool parse_instruction_tag(const char* str, ptx_instruction_tag* instruction_tag) {
    if (!str || !instruction_tag) {
        return false;
    }

    // Perform a linear search through the instruction map
    for (int i = 0; i < instruction_map_size; i++) {
        if (str_case_cmp(str, instruction_map[i].name) == 0) {
            *instruction_tag = instruction_map[i].instruction;
            return true;
        }
    }

    // Instruction not found
    return false;
}

/**
 * Parse a predicate expression and create a ptx_predicate_t
 */
bool parse_predicate(const char* str, ptx_predicate_t** predicate) {
    if (!str || !predicate || str[0] != '@') {
        return false;
    }

    // Allocate memory for the predicate
    *predicate = (ptx_predicate_t*)malloc(sizeof(ptx_predicate_t));
    if (!*predicate) {
        return false; // Memory allocation failed
    }

    // Initialize with default values
    (*predicate)->name = NULL;
    (*predicate)->negated = false;

    // Move past the @ symbol
    const char* current = str + 1;

    // Check for negation (!)
    if (*current == '!') {
        (*predicate)->negated = true;
        current++;
    }

    // Ensure we have a register symbol (%)
    if (*current != '%') {
        free(*predicate);
        *predicate = NULL;
        return false;
    }

    // Extract the predicate name
    const char* name_start = current;
    while (*current && !isspace(*current) && *current != ';') {
        current++;
    }

    // Create a copy of the name including the % symbol
    size_t name_length = current - name_start;
    (*predicate)->name = (char*)malloc(name_length + 1);
    if (!(*predicate)->name) {
        free(*predicate);
        *predicate = NULL;
        return false; // Memory allocation failed
    }

    strncpy((*predicate)->name, name_start, name_length);
    (*predicate)->name[name_length] = '\0';

    return true;
}

/**
 * Parse a full instruction with optional predicate
 */
bool parse_full_instruction(const char* str, ptx_instruction_t** instruction) {
    if (!str || !instruction) {
        return false;
    }

    // Skip leading whitespace
    const char* current = str;
    while (*current && isspace(*current)) {
        current++;
    }

    // Allocate memory for the instruction
    *instruction = (ptx_instruction_t*)malloc(sizeof(ptx_instruction_t));
    if (!*instruction) {
        return false; // Memory allocation failed
    }

    // Initialize with NULL predicate
    (*instruction)->predicate = NULL;

    // Check if the instruction starts with a predicate (@)
    if (*current == '@') {
        // Parse the predicate
        if (!parse_predicate(current, &(*instruction)->predicate)) {
            free(*instruction);
            *instruction = NULL;
            return false;
        }

        // Skip past the predicate to find the instruction
        while (*current && !isspace(*current)) {
            current++;
        }
        
        // Skip any whitespace between predicate and instruction
        while (*current && isspace(*current)) {
            current++;
        }
    }

    // Extract the instruction name
    const char* instr_start = current;
    while (*current && !isspace(*current) && *current != '.' && *current != ';') {
        current++;
    }

    // Create a temporary buffer for the instruction name
    size_t instr_length = current - instr_start;
    char* instr_name = (char*)malloc(instr_length + 1);
    if (!instr_name) {
        if ((*instruction)->predicate) {
            free((*instruction)->predicate->name);
            free((*instruction)->predicate);
        }
        free(*instruction);
        *instruction = NULL;
        return false; // Memory allocation failed
    }

    strncpy(instr_name, instr_start, instr_length);
    instr_name[instr_length] = '\0';

    // Parse the instruction tag
    bool instruction_parsed = parse_instruction_tag(instr_name, &(*instruction)->tag);
    free(instr_name); // We don't need this anymore

    if (!instruction_parsed) {
        if ((*instruction)->predicate) {
            free((*instruction)->predicate->name);
            free((*instruction)->predicate);
        }
        free(*instruction);
        *instruction = NULL;
        return false;
    }

    return true;
}

// For backwards compatibility
bool parse_instruction(const char* str, ptx_instruction_t* instruction) {
    ptx_instruction_t* new_instruction = NULL;
    bool result = parse_full_instruction(str, &new_instruction);
    
    if (result && new_instruction) {
        // Copy the tag to the old-style instruction
        instruction->tag = new_instruction->tag;
        instruction->predicate = NULL;  // Don't copy the predicate
        
        // Free the allocated instruction
        if (new_instruction->predicate) {
            free(new_instruction->predicate->name);
            free(new_instruction->predicate);
        }
        free(new_instruction);
        return true;
    }
    
    return false;
} 