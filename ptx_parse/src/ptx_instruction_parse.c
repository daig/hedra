#include <ptx_parse/ptx_instruction_parse.h>
#include <ptx_parse/ptx_memory_operand_parse.h>
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
 * Parse instruction modifiers from a string
 */
bool parse_instruction_modifiers(const char* str, ptx_instruction_t* instruction, int* chars_read) {
    if (!str || !instruction || !chars_read) {
        return false;
    }
    
    // Initialize the number of characters read
    *chars_read = 0;
    
    // Skip leading whitespace
    const char* current = str;
    while (*current && isspace(*current)) {
        current++;
    }
    
    // Check if we have modifiers
    while (*current == '.') {
        // Move past the dot
        current++;
        
        // Extract the modifier
        const char* modifier_start = current;
        while (*current && !isspace(*current) && *current != '.' && *current != ',') {
            current++;
        }
        
        // Create a copy of the modifier
        size_t modifier_length = current - modifier_start;
        char* modifier = (char*)malloc(modifier_length + 1);
        if (!modifier) {
            return false; // Memory allocation failed
        }
        
        strncpy(modifier, modifier_start, modifier_length);
        modifier[modifier_length] = '\0';
        
        // Add the modifier to the instruction
        if (!add_instruction_modifier(instruction, modifier)) {
            free(modifier);
            return false;
        }
        
        // Free the temporary modifier
        free(modifier);
        
        // Skip whitespace
        while (*current && isspace(*current)) {
            current++;
        }
    }
    
    // Update the number of characters read
    *chars_read = current - str;
    
    return true;
}

/**
 * Parse instruction operands from a string
 */
bool parse_instruction_operands(const char* str, ptx_instruction_t* instruction, int* chars_read) {
    if (!str || !instruction || !chars_read) {
        return false;
    }
    
    // Initialize the number of characters read
    *chars_read = 0;
    
    // Skip leading whitespace
    const char* current = str;
    while (*current && isspace(*current)) {
        current++;
    }
    
    // Check if we have operands
    if (!*current) {
        return true; // No operands
    }
    
    // Parse the destination operand (if any)
    const char* dest_start = current;
    while (*current && *current != ',' && *current != ';' && !isspace(*current)) {
        current++;
    }
    
    // Create a copy of the destination operand
    size_t dest_length = current - dest_start;
    if (dest_length > 0) {
        char* dest = (char*)malloc(dest_length + 1);
        if (!dest) {
            return false; // Memory allocation failed
        }
        
        strncpy(dest, dest_start, dest_length);
        dest[dest_length] = '\0';
        
        // Set the destination operand
        if (!set_instruction_dest(instruction, dest)) {
            free(dest);
            return false;
        }
        
        // Free the temporary destination
        free(dest);
    }
    
    // Skip whitespace
    while (*current && isspace(*current)) {
        current++;
    }
    
    // Check if we have a comma
    if (*current == ',') {
        // Move past the comma
        current++;
        
        // Skip whitespace
        while (*current && isspace(*current)) {
            current++;
        }
        
        // Parse the source operands
        while (*current && *current != ';') {
            // Check if we have a memory operand
            if (*current == '[' || (isalpha(*current) && strchr(current, '['))) {
                // Parse the memory operand
                ptx_memory_operand_t* memory_operand = NULL;
                int mem_chars_read = 0;
                if (!parse_memory_operand(current, &memory_operand, &mem_chars_read)) {
                    return false;
                }
                
                // Add the memory operand to the instruction
                if (!add_memory_operand(instruction, memory_operand)) {
                    free_memory_operand(memory_operand);
                    return false;
                }
                
                // Update the current position
                current += mem_chars_read;
            } else {
                // Parse the register operand
                const char* reg_start = current;
                while (*current && *current != ',' && *current != ';' && !isspace(*current)) {
                    current++;
                }
                
                // Create a copy of the register operand
                size_t reg_length = current - reg_start;
                if (reg_length > 0) {
                    char* reg = (char*)malloc(reg_length + 1);
                    if (!reg) {
                        return false; // Memory allocation failed
                    }
                    
                    strncpy(reg, reg_start, reg_length);
                    reg[reg_length] = '\0';
                    
                    // Add the register operand to the instruction
                    if (!add_register_operand(instruction, reg)) {
                        free(reg);
                        return false;
                    }
                    
                    // Free the temporary register
                    free(reg);
                }
            }
            
            // Skip whitespace
            while (*current && isspace(*current)) {
                current++;
            }
            
            // Check if we have a comma
            if (*current == ',') {
                // Move past the comma
                current++;
                
                // Skip whitespace
                while (*current && isspace(*current)) {
                    current++;
                }
            } else {
                break;
            }
        }
    }
    
    // Update the number of characters read
    *chars_read = current - str;
    
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

    // Initialize the instruction
    (*instruction)->predicate = NULL;
    (*instruction)->modifiers = NULL;
    (*instruction)->dest_operand = NULL;
    (*instruction)->num_operands = 0;
    for (int i = 0; i < MAX_INSTRUCTION_OPERANDS; i++) {
        (*instruction)->reg_operands[i] = NULL;
        (*instruction)->is_mem_operand[i] = false;
    }

    // Check if the instruction starts with a predicate (@)
    if (*current == '@') {
        // Parse the predicate
        if (!parse_predicate(current, &(*instruction)->predicate)) {
            free_instruction(*instruction);
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
        free_instruction(*instruction);
        *instruction = NULL;
        return false; // Memory allocation failed
    }

    strncpy(instr_name, instr_start, instr_length);
    instr_name[instr_length] = '\0';

    // Parse the instruction tag
    if (!parse_instruction_tag(instr_name, &(*instruction)->tag)) {
        free(instr_name);
        free_instruction(*instruction);
        *instruction = NULL;
        return false;
    }

    // Free the temporary instruction name
    free(instr_name);

    // Parse modifiers
    int modifier_chars_read = 0;
    if (!parse_instruction_modifiers(current, *instruction, &modifier_chars_read)) {
        free_instruction(*instruction);
        *instruction = NULL;
        return false;
    }
    
    // Update the current position
    current += modifier_chars_read;
    
    // Parse operands
    int operand_chars_read = 0;
    if (!parse_instruction_operands(current, *instruction, &operand_chars_read)) {
        free_instruction(*instruction);
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