#include "ptx_type_fundamental.h"
#include "ptx_instruction.h"
#include <stdbool.h>

/**
 * Check if a given type is restricted to only ld, st, and cvt instructions
 * (u8, s8, and b8 types have this restriction)
 */
bool ptx_type_restricted_to_load_store_convert(ptx_fundamental_type_t type) {
    return (type == PTX_TYPE_U8 || type == PTX_TYPE_S8 || type == PTX_TYPE_B8);
}

/**
 * Check if a type is a floating point half-precision type (f16 or f16x2)
 */
bool ptx_type_is_half_precision(ptx_fundamental_type_t type) {
    return (type == PTX_TYPE_F16 || type == PTX_TYPE_F16X2);
}

/**
 * Check if an instruction is a half-precision floating point instruction
 * Note: This is a simplified implementation - actual implementation would 
 * need to identify all half-precision floating point instructions
 */
bool ptx_instruction_is_half_precision(ptx_instruction_t instruction) {
    // This is a simplified implementation - in reality, a more detailed
    // list of half-precision instructions would be needed
    // For now, we'll assume certain arithmetic operations can be half-precision
    switch (instruction) {
        case FMA:
        case MAD:
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case RCP:
        case SQRT:
        case NEG:
        case ABS:
        case MIN:
        case MAX:
            return true;
        default:
            return false;
    }
}

/**
 * Check if an instruction is a texture fetch instruction
 */
bool ptx_instruction_is_texture_fetch(ptx_instruction_t instruction) {
    return (instruction == TEX || instruction == TLD4 || instruction == TXQ);
}

/**
 * Check if an instruction is a conversion instruction
 */
bool ptx_instruction_is_conversion(ptx_instruction_t instruction) {
    return (instruction == CVT);
}

/**
 * Check if type is allowed for conversion to/from f32, f64
 */
bool ptx_type_allowed_for_float_conversion(ptx_fundamental_type_t type) {
    // f16 can be converted to/from f32 and f64
    return (type == PTX_TYPE_F16 || type == PTX_TYPE_F32 || type == PTX_TYPE_F64);
}

/**
 * Main function to check if a type is compatible with an instruction
 * based on the PTX specification
 */
bool ptx_type_compatible_with_instruction(ptx_fundamental_type_t type, ptx_instruction_t instruction) {
    // First check for restricted types (u8, s8, b8)
    if (ptx_type_restricted_to_load_store_convert(type)) {
        return (instruction == LD || instruction == ST || instruction == CVT);
    }
    
    // Check f16 compatibility rules
    if (type == PTX_TYPE_F16) {
        // f16 is allowed in conversions with f32/f64
        if (instruction == CVT) {
            return true; // The actual conversion target would need to be checked elsewhere
        }
        
        // f16 is allowed in half precision floating point and texture instructions
        return ptx_instruction_is_half_precision(instruction) || 
               ptx_instruction_is_texture_fetch(instruction);
    }
    
    // Check f16x2 compatibility rules
    if (type == PTX_TYPE_F16X2) {
        return ptx_instruction_is_half_precision(instruction) ||
               ptx_instruction_is_texture_fetch(instruction);
    }
    
    // For all other types, we assume they're generally compatible with most instructions
    // except for specific restrictions that would be added here
    
    // Note: This implementation doesn't handle the case where instructions permit
    // wider source/destination operands - that would require tracking operand positions
    // which is beyond the scope of this basic compatibility check
    
    return true;
} 