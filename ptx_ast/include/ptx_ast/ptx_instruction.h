#pragma once
#include <stdbool.h>
#include <ptx_ast/ptx_memory_operand.h>

typedef enum ptx_instruction_tag { 
    ABS,
    CVTA,
    MEMBAR,
    SETP,
    VABSDIFF4,
    ACTIVEMASK,
    DISCARD,
    MIN,
    SHF,
    VADD,
    ADD,
    DIV,
    MMA,
    SHFL,
    VADD2,
    ADDC,
    DP2A,
    MOV,
    SHL,
    VADD4,
    ALLOCA,
    DP4A,
    MOVMATRIX,
    SHR,
    VAVRG2,
    AND,
    ELECT,
    MUL,
    SIN,
    VAVRG4,
    APPLYPRIORITY,
    EX2,
    MUL24,
    SLCT,
    VMAD,
    ATOM,
    EXIT,
    MULTIMEM,
    SQRT,
    VMAX,
    BAR,
    FENCE,
    NANOSLEEP,
    ST,
    VMAX2,
    BARRIER,
    FMA,
    NEG,
    STACKRESTORE,
    VMAX4,
    BFE,
    FNS,
    NOT,
    STACKSAVE,
    VMIN,
    BFI,
    GETCTARANK,
    OR,
    STMATRIX,
    VMIN2,
    BFIND,
    GRIDDEPCONTROL,
    PMEVENT,
    SUB,
    VMIN4,
    BMSK,
    ISSPACEP,
    POPC,
    SUBC,
    VOTE,
    BRA,
    ISTYPEP,
    PREFETCH,
    SULD,
    VSET,
    BREV,
    LD,
    PREFETCHU,
    SUQ,
    VSET2,
    BRKPT,
    LDMATRIX,
    PRMT,
    SURED,
    VSET4,
    BRX,
    LDU,
    RCP,
    SUST,
    VSHL,
    CALL,
    LG2,
    RED,
    SZEXT,
    VSHR,
    CLZ,
    LOP3,
    REDUX,
    TANH,
    VSUB,
    CNOT,
    MAD,
    REM,
    TESTP,
    VSUB2,
    COPYSIGN,
    MAD24,
    RET,
    TEX,
    VSUB4,
    COS,
    MADC,
    RSQRT,
    TLD4,
    WGMMA,
    CLUSTERLAUNCHCONTROL,
    MAPA,
    SAD,
    TRAP,
    WMMA,
    CP,
    MATCH,
    SELP,
    TXQ,
    XOR,
    CREATEPOLICY,
    MAX,
    SET,
    VABSDIFF,
    CVT,
    MBARRIER,
    SETMAXNREG,
    VABSDIFF2

} ptx_instruction_tag;

// Structure to represent a predicate
typedef struct ptx_predicate_t {
    char* name;  // The name of the predicate register (e.g., "%p_out_of_bounds")
    bool negated; // Whether the predicate is negated (e.g., "!%p_out_of_bounds")
} ptx_predicate_t;

// Structure to represent instruction modifiers (like .const, .s32, etc.)
typedef struct ptx_instruction_modifier_t {
    char* modifier;  // The modifier text (e.g., "const", "s32")
    struct ptx_instruction_modifier_t* next;  // Linked list for multiple modifiers
} ptx_instruction_modifier_t;

// Maximum number of operands an instruction can have
#define MAX_INSTRUCTION_OPERANDS 4

// Structure to represent a full instruction with optional predicate
typedef struct ptx_instruction_t {
    ptx_instruction_tag tag;     // The type of instruction
    ptx_predicate_t* predicate;  // Optional predicate (NULL if not predicated)
    
    // Instruction modifiers (like .const, .s32, etc.)
    ptx_instruction_modifier_t* modifiers;
    
    // Destination operand (for instructions that write to a register)
    char* dest_operand;
    
    // Source operands
    int num_operands;
    union {
        char* reg_operands[MAX_INSTRUCTION_OPERANDS];  // For register operands
        ptx_memory_operand_t* mem_operands[MAX_INSTRUCTION_OPERANDS];  // For memory operands
    };
    
    // Flags to indicate which operands are memory operands
    bool is_mem_operand[MAX_INSTRUCTION_OPERANDS];
} ptx_instruction_t;

// Function to create a new instruction modifier
ptx_instruction_modifier_t* create_instruction_modifier(const char* modifier_text);

// Function to free an instruction modifier list
void free_instruction_modifiers(ptx_instruction_modifier_t* modifiers);

// Function to add a modifier to an instruction
bool add_instruction_modifier(ptx_instruction_t* instruction, const char* modifier_text);

// Function to add a register operand to an instruction
bool add_register_operand(ptx_instruction_t* instruction, const char* operand_text);

// Function to add a memory operand to an instruction
bool add_memory_operand(ptx_instruction_t* instruction, ptx_memory_operand_t* memory_operand);

// Function to set the destination operand of an instruction
bool set_instruction_dest(ptx_instruction_t* instruction, const char* dest_text);

// Function to free an instruction
void free_instruction(ptx_instruction_t* instruction);

