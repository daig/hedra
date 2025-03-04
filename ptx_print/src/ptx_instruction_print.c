#include <ptx_print/ptx_instruction_print.h>
#include <ptx_print/ptx_memory_operand_print.h>
#include <string.h>
#include <stdlib.h>

// Struct to map instruction enum values to strings
// This is the inverse of the mapping in ptx_instruction_parse.c
typedef struct {
    const char* name;
    ptx_instruction_tag instruction;
} instruction_map_entry;

// Array of instruction mappings
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
    {"activemask", ACTIVEMASK},
    {"discard", DISCARD},
    {"mma", MMA},
    {"shf", SHF},
    {"vadd2", VADD2},
    {"alloca", ALLOCA},
    {"movmatrix", MOVMATRIX},
    {"vavrg2", VAVRG2},
    {"elect", ELECT},
    {"vavrg4", VAVRG4},
    {"applypriority", APPLYPRIORITY},
    {"multimem", MULTIMEM},
    {"vmax", VMAX},
    {"barrier", BARRIER},
    {"stackrestore", STACKRESTORE},
    {"vmax2", VMAX2},
    {"vmax4", VMAX4},
    {"getctarank", GETCTARANK},
    {"stmatrix", STMATRIX},
    {"vmin", VMIN},
    {"griddepcontrol", GRIDDEPCONTROL},
    {"vmin2", VMIN2},
    {"vmin4", VMIN4},
    {"szext", SZEXT},
    {"vshr", VSHR},
    {"redux", REDUX},
    {"tanh", TANH},
    {"vsub", VSUB},
    {"vsub2", VSUB2},
    {"vsub4", VSUB4},
    {"wgmma", WGMMA},
    {"clusterlaunchcontrol", CLUSTERLAUNCHCONTROL},
    {"mapa", MAPA},
    {"wmma", WMMA},
    {"match", MATCH},
    {"createpolicy", CREATEPOLICY},
    {"vabsdiff2", VABSDIFF2},
    {"mbarrier", MBARRIER},
    {"setmaxnreg", SETMAXNREG},
    {"vabsdiff4", VABSDIFF4},
    {"fence", FENCE},
    {"nanosleep", NANOSLEEP},
    {"stacksave", STACKSAVE},
    {"ldmatrix", LDMATRIX},
    {"pmevent", PMEVENT},
    {"lop3", LOP3},
    {"vset", VSET},
    {"vset2", VSET2},
    {"vset4", VSET4},
    {"vshl", VSHL},
    {"brx", BRX},
};

static const size_t instruction_map_size = sizeof(instruction_map) / sizeof(instruction_map[0]);

/**
 * Gets the string representation of a PTX instruction tag
 */
const char* get_instruction_string(ptx_instruction_tag tag) {
    for (size_t i = 0; i < instruction_map_size; i++) {
        if (instruction_map[i].instruction == tag) {
            return instruction_map[i].name;
        }
    }
    return NULL;  // Instruction not found
}

/**
 * Prints a PTX predicate to a file stream
 */
bool print_predicate_to_file(FILE* file, const ptx_predicate_t* predicate) {
    if (!file || !predicate || !predicate->name) {
        return false;
    }
    
    fprintf(file, "@");
    if (predicate->negated) {
        fprintf(file, "!");
    }
    fprintf(file, "%s ", predicate->name);
    return true;
}

/**
 * Prints a PTX predicate to a string buffer
 */
int print_predicate_to_buffer(char* buffer, size_t buffer_size, const ptx_predicate_t* predicate) {
    if (!buffer || buffer_size == 0 || !predicate || !predicate->name) {
        return -1;
    }
    
    char tmp[buffer_size];
    int chars = 0;
    
    // Start with @
    tmp[chars++] = '@';
    
    // Add ! if negated
    if (predicate->negated) {
        tmp[chars++] = '!';
    }
    
    // Add the predicate name
    int name_len = snprintf(tmp + chars, buffer_size - chars, "%s ", predicate->name);
    if (name_len < 0 || name_len >= buffer_size - chars) {
        return -1;
    }
    chars += name_len;
    
    // Copy to output buffer
    strncpy(buffer, tmp, chars);
    buffer[chars] = '\0';
    
    return chars;
}

/**
 * Print instruction modifiers to a file stream
 */
bool print_instruction_modifiers_to_file(FILE* file, const ptx_instruction_modifier_t* modifiers) {
    if (!file) {
        return false;
    }
    
    // If no modifiers, return success
    if (!modifiers) {
        return true;
    }
    
    // Print each modifier in reverse order (since they were added to the beginning of the list)
    // First, count the modifiers
    int count = 0;
    const ptx_instruction_modifier_t* current = modifiers;
    while (current) {
        count++;
        current = current->next;
    }
    
    // Create an array to store the modifiers in reverse order
    const char** modifier_array = (const char**)malloc(count * sizeof(char*));
    if (!modifier_array) {
        return false;
    }
    
    // Fill the array
    current = modifiers;
    for (int i = 0; i < count; i++) {
        modifier_array[count - i - 1] = current->modifier;
        current = current->next;
    }
    
    // Print the modifiers
    for (int i = 0; i < count; i++) {
        fprintf(file, ".%s", modifier_array[i]);
        if (i < count - 1) {
            fprintf(file, " ");
        }
    }
    
    // Free the array
    free(modifier_array);
    
    return true;
}

/**
 * Print instruction modifiers to a string buffer
 */
int print_instruction_modifiers_to_buffer(char* buffer, size_t buffer_size, const ptx_instruction_modifier_t* modifiers) {
    if (!buffer || buffer_size == 0) {
        return -1;
    }
    
    // If no modifiers, return success
    if (!modifiers) {
        buffer[0] = '\0';
        return 0;
    }
    
    // Print each modifier in reverse order (since they were added to the beginning of the list)
    // First, count the modifiers
    int count = 0;
    const ptx_instruction_modifier_t* current = modifiers;
    while (current) {
        count++;
        current = current->next;
    }
    
    // Create an array to store the modifiers in reverse order
    const char** modifier_array = (const char**)malloc(count * sizeof(char*));
    if (!modifier_array) {
        return -1;
    }
    
    // Fill the array
    current = modifiers;
    for (int i = 0; i < count; i++) {
        modifier_array[count - i - 1] = current->modifier;
        current = current->next;
    }
    
    // Print the modifiers
    int chars = 0;
    for (int i = 0; i < count; i++) {
        int mod_len = snprintf(buffer + chars, buffer_size - chars, ".%s", modifier_array[i]);
        if (mod_len < 0 || mod_len >= buffer_size - chars) {
            free(modifier_array);
            return -1;
        }
        chars += mod_len;
        
        if (i < count - 1) {
            if (chars < buffer_size - 1) {
                buffer[chars++] = ' ';
                buffer[chars] = '\0';
            } else {
                free(modifier_array);
                return -1;
            }
        }
    }
    
    // Free the array
    free(modifier_array);
    
    return chars;
}

/**
 * Print instruction operands to a file stream
 */
bool print_instruction_operands_to_file(FILE* file, const ptx_instruction_t* instruction) {
    if (!file || !instruction) {
        return false;
    }
    
    // Print the destination operand if it exists
    if (instruction->dest_operand) {
        fprintf(file, " %s", instruction->dest_operand);
    }
    
    // Print the source operands if they exist
    if (instruction->num_operands > 0) {
        fprintf(file, ", ");
        
        for (int i = 0; i < instruction->num_operands; i++) {
            if (instruction->is_mem_operand[i]) {
                if (!print_memory_operand_to_file(file, instruction->mem_operands[i])) {
                    return false;
                }
            } else {
                fprintf(file, "%s", instruction->reg_operands[i]);
            }
            
            if (i < instruction->num_operands - 1) {
                fprintf(file, ", ");
            }
        }
    }
    
    return true;
}

/**
 * Print instruction operands to a string buffer
 */
int print_instruction_operands_to_buffer(char* buffer, size_t buffer_size, const ptx_instruction_t* instruction) {
    if (!buffer || buffer_size == 0 || !instruction) {
        return -1;
    }
    
    int chars = 0;
    
    // Print the destination operand if it exists
    if (instruction->dest_operand) {
        int dest_len = snprintf(buffer + chars, buffer_size - chars, " %s", instruction->dest_operand);
        if (dest_len < 0 || dest_len >= buffer_size - chars) {
            return -1;
        }
        chars += dest_len;
    }
    
    // Print the source operands if they exist
    if (instruction->num_operands > 0) {
        int comma_len = snprintf(buffer + chars, buffer_size - chars, ", ");
        if (comma_len < 0 || comma_len >= buffer_size - chars) {
            return -1;
        }
        chars += comma_len;
        
        for (int i = 0; i < instruction->num_operands; i++) {
            if (instruction->is_mem_operand[i]) {
                int mem_chars = print_memory_operand_to_buffer(buffer + chars, buffer_size - chars, instruction->mem_operands[i]);
                if (mem_chars < 0) {
                    return -1;
                }
                chars += mem_chars;
            } else {
                int reg_len = snprintf(buffer + chars, buffer_size - chars, "%s", instruction->reg_operands[i]);
                if (reg_len < 0 || reg_len >= buffer_size - chars) {
                    return -1;
                }
                chars += reg_len;
            }
            
            if (i < instruction->num_operands - 1) {
                int sep_len = snprintf(buffer + chars, buffer_size - chars, ", ");
                if (sep_len < 0 || sep_len >= buffer_size - chars) {
                    return -1;
                }
                chars += sep_len;
            }
        }
    }
    
    return chars;
}

/**
 * Prints a PTX instruction to a file stream
 */
bool print_instruction_to_file(FILE* file, const ptx_instruction_t instruction) {
    if (!file) {
        return false;
    }
    
    // Print predicate if present
    if (instruction.predicate) {
        if (!print_predicate_to_file(file, instruction.predicate)) {
            return false;
        }
    }
    
    // Get and print the instruction name
    const char* instruction_str = get_instruction_string(instruction.tag);
    if (!instruction_str) {
        return false;
    }
    
    // Print the instruction
    fprintf(file, "%s", instruction_str);
    
    // Print modifiers if present
    if (instruction.modifiers) {
        if (!print_instruction_modifiers_to_file(file, instruction.modifiers)) {
            return false;
        }
    }
    
    // Print operands if present
    if (!print_instruction_operands_to_file(file, &instruction)) {
        return false;
    }
    
    return true;
}

/**
 * Prints a PTX instruction to a string buffer
 */
int print_instruction_to_buffer(char* buffer, size_t buffer_size, const ptx_instruction_t instruction) {
    if (!buffer || buffer_size == 0) {
        return -1;
    }
    
    int total_chars = 0;
    
    // Print predicate if present
    if (instruction.predicate) {
        int pred_chars = print_predicate_to_buffer(buffer, buffer_size, instruction.predicate);
        if (pred_chars < 0) {
            return -1;
        }
        total_chars += pred_chars;
    }
    
    // Get the instruction name
    const char* instruction_str = get_instruction_string(instruction.tag);
    if (!instruction_str) {
        return -1;
    }
    
    // Print the instruction name
    int instr_len = snprintf(buffer + total_chars, buffer_size - total_chars, "%s", instruction_str);
    if (instr_len < 0 || instr_len >= buffer_size - total_chars) {
        return -1;
    }
    total_chars += instr_len;
    
    // Print modifiers if present
    if (instruction.modifiers) {
        int mod_chars = print_instruction_modifiers_to_buffer(buffer + total_chars, buffer_size - total_chars, instruction.modifiers);
        if (mod_chars < 0) {
            return -1;
        }
        total_chars += mod_chars;
    }
    
    // Print operands if present
    int op_chars = print_instruction_operands_to_buffer(buffer + total_chars, buffer_size - total_chars, &instruction);
    if (op_chars < 0) {
        return -1;
    }
    total_chars += op_chars;
    
    return total_chars;
} 