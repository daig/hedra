#pragma once

#include "ptx_instruction.h"

typedef enum {
    // Signed integer types
    PTX_TYPE_S8,
    PTX_TYPE_S16,
    PTX_TYPE_S32,
    PTX_TYPE_S64,

    // Unsigned integer types
    PTX_TYPE_U8,
    PTX_TYPE_U16,
    PTX_TYPE_U32,
    PTX_TYPE_U64,

    // Floating-point types
    PTX_TYPE_F16,
    PTX_TYPE_F16X2,
    PTX_TYPE_F32,
    PTX_TYPE_F64,

    // Bits (untyped)
    PTX_TYPE_B8,
    PTX_TYPE_B16,
    PTX_TYPE_B32,
    PTX_TYPE_B64,
    PTX_TYPE_B128,

    // Predicate type
    PTX_TYPE_PRED
} ptx_fundamental_type_t;

// Type compatibility checking functions
// Check if a given type is restricted to only ld, st, and cvt instructions
bool ptx_type_restricted_to_load_store_convert(ptx_fundamental_type_t type);

// Check if a type is compatible with a specific instruction
bool ptx_type_compatible_with_instruction(ptx_fundamental_type_t type, ptx_instruction_t instruction);

// Check if a type is a floating point half-precision type (f16 or f16x2)
bool ptx_type_is_half_precision(ptx_fundamental_type_t type);

// Check if an instruction is a half-precision floating point instruction
bool ptx_instruction_is_half_precision(ptx_instruction_t instruction);

// Check if an instruction is a texture fetch instruction
bool ptx_instruction_is_texture_fetch(ptx_instruction_t instruction);

// Check if an instruction is a conversion instruction
bool ptx_instruction_is_conversion(ptx_instruction_t instruction);

// Check if type is allowed for conversion to/from f32, f64
bool ptx_type_allowed_for_float_conversion(ptx_fundamental_type_t type);

