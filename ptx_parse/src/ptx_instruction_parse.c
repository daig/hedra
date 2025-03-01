#include <ptx_parse/ptx_instruction_parse.h>
#include <string.h>
#include <ctype.h>

// Struct to map instruction strings to enum values
typedef struct {
    const char* name;
    ptx_instruction_t instruction;
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
 * Parse a PTX instruction string and store in ptx_instruction_t
 * 
 * This function takes a string representing a PTX instruction (e.g., "add", "mul", "ld"),
 * and determines the corresponding instruction type from the ptx_instruction_t enum.
 *
 * @param str The instruction string to parse
 * @param instruction Pointer to ptx_instruction_t where the result will be stored
 * @return true if successful, false if the instruction is not recognized
 */
bool parse_instruction(const char* str, ptx_instruction_t* instruction) {
    if (!str || !instruction) {
        return false;
    }

    // Perform a linear search through the instruction map
    for (int i = 0; i < instruction_map_size; i++) {
        if (str_case_cmp(str, instruction_map[i].name) == 0) {
            *instruction = instruction_map[i].instruction;
            return true;
        }
    }

    // Instruction not found
    return false;
} 